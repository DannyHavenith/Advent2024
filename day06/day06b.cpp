#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include "../timer.h"

namespace {
    using Map = std::vector< std::string>;

    constexpr auto maxDirection = 3;
    constexpr auto ghostBit = 4;
    constexpr auto maxGhostDirection = maxDirection | ghostBit;

    struct Position
    {
        std::uint8_t row{};
        std::uint8_t col{};
        auto operator<=>( const Position &) const = default;
    };

    void PrintMap( const Map &m, const Position &pos)
    {
        return;
        const std::uint8_t mapRows = m.size();
        assert( mapRows >= 1);

        const std::uint8_t mapColumns = m[0].size();
        const auto rows = std::min( mapRows, std::uint8_t(41));
        const auto columns = std::min( std::uint8_t(81), mapColumns);

        const auto rmid = rows/2;
        const auto cmid = columns/2;

        const std::uint8_t firstRow = pos.row >= rmid ? std::min( pos.row - rmid , mapRows - rows) : 0;
        const std::uint8_t firstColumn = pos.col >= cmid ? std::min( pos.col - cmid, mapColumns - columns) : 0;

        const auto Translate = [](char c) -> const char * {
            static char buffer[] = {0,0};
            switch (c)
            {
                case 0: return "^";
                case 1: return ">";
                case 2: return "v";
                case 3: return "<";

                case 4: return "\033[36m^\033[0m";
                case 5: return "\033[36m>\033[0m";
                case 6: return "\033[36mv\033[0m";
                case 7: return "\033[36m<\033[0m";
                case '.': return " ";

                default:
                    buffer[0] = c;
                    return buffer;
            }
        };

        std::cout << "\033[H";

        for (auto row = firstRow; row < firstRow + rows; ++row)
        {
            for (auto col = firstColumn; col < firstColumn + columns; ++col)
            {
                if (pos == Position{ row, col})
                {
                    std::cout << "\033[1;31m*\033[0m";
                }
                else
                {
                    std::cout << Translate( m[row][col]);
                }
            }
            std::cout << '\n';
        }
        std::cout << '\n';

        using namespace std::literals;
        std::this_thread::sleep_for( 40ms);
    }

    struct Guard
    {
        Position position;

        std::uint8_t direction{0}; // 0 - up, 1 - right, etc.
        std::uint8_t Position::*heading = &Position::row;
        std::int8_t step = -1;

        Guard( Position pos, std::uint8_t dir = 0)
        : position{ std::move(pos)}
        {
            SetDirection( dir);
        }

        void Rotate()
        {
            SetDirection( (direction + 1) %  4);
        }

        void SetDirection( std::uint8_t newDirection)
        {
            direction = newDirection;
            heading = (direction & 0x01) ? &Position::col : &Position::row;
            step = ((direction ^ (direction >> 1)) & 0x01) ? 1 : -1;
        }

        Position Step( Position pos)
        {
            pos.*heading += step;
            return pos;
        }

        static char &Peek( const Position &pos, Map &map)
        {
            return map[pos.row][pos.col];
        }

        static const char &Peek( const Position &pos, const Map &map)
        {
            return map[pos.row][pos.col];
        }
    };

    std::size_t WouldLoop( Guard &g, Map &map)
    {
        const auto rows = map.size();
        assert( rows > 0);
        const auto columns = map[0].size();

        while (g.position.col < columns and g.position.row < rows)
        {
            auto &currentCell = g.Peek( g.position, map);

            if (currentCell > maxGhostDirection)
            {
                currentCell = g.direction | ghostBit;
            }
            else if ((currentCell & ~ghostBit) == g.direction)
            {
                return true;
            }

            Position nextPos = g.Step( g.position);
            if (nextPos.col >= columns or nextPos.row >= rows) return false;

            if (g.Peek( nextPos, map) == '#')
            {
                g.Rotate();
            }
            else
            {
                g.position = nextPos;
            }
        }

        return false;
    }

    std::size_t WalkGuard( Guard g, Map map)
    {
        const auto rows = map.size();
        assert( rows > 0);
        const auto columns = map[0].size();
        std::size_t opportunities = 0;

        while (g.position.col < columns and g.position.row < rows)
        {

            auto &currentCell = g.Peek( g.position, map);
            if (currentCell > maxDirection)
            {
                currentCell = g.direction;
            }
            else if (currentCell == g.direction)
            {
                return 0; // something's wrong: we're looping already.
            }

            Position nextPos = g.Step( g.position);
            if (nextPos.col < columns and nextPos.row < rows and g.Peek( nextPos, map) == '#')
            {
                g.Rotate();
            }
            else
            {
                // if we haven't travelled through the next cell before, see if
                // blocking it would get us in a loop.
                if (nextPos.row < rows and nextPos.col < columns and g.Peek( nextPos, map) > maxDirection)
                {
                    // create an alternative reality where there is an obstacle
                    // in the way and we have turned right.
                    auto ghost = g;
                    ghost.Rotate();
                    auto ghostMap = map;
                    ghostMap[nextPos.row][nextPos.col] = '#';

                    if (WouldLoop( ghost, ghostMap))
                    {
                        ++opportunities;
                    };

                    ghost.Peek( nextPos, ghostMap) = ' ';
                    PrintMap( ghostMap, g.position);
                }

                g.position = nextPos;
            }
        }

        return opportunities;
    }
}

int main()
{
    Timer _;
    std::ifstream input{"input06.txt"};
    std::string line;

    using namespace std::literals;

    // read the map
    std::vector<std::string> map;
    char dir = '^';
    Position position;
    std::uint8_t rows = 0;
    std::uint8_t columns = 0;
    while (getline( input, line))
    {
        columns = std::max( columns, static_cast<std::uint8_t>( line.size()));
        map.push_back( line);
        if (const auto col = line.find(dir); col != line.npos)
        {
            position = {rows, static_cast<std::uint8_t>( col)};
        }
        ++rows;
    }

    // walk the guard
    auto opportunities = WalkGuard({position},  std::move( map));

    std::cout << "opportunities: " << opportunities << '\n';
}


