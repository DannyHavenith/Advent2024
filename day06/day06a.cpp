#include <algorithm>
#include <iostream>
#include <fstream>
#include <ranges>
#include <set>
#include <string>
#include <vector>

#include "../timer.h"

namespace {
    using Map = std::vector< std::string>;

    struct Position
    {
        std::size_t row{};
        std::size_t col{};
    };

    struct Guard
    {
        Position position;

        std::uint8_t direction{0}; // 0 - up, 1 - right, etc.
        std::size_t Position::*heading = &Position::row;
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
    };
}

int main()
{
    Timer _;
    std::ifstream input{"input06.txt"};
    std::string line;


    // read the map
    std::vector<std::string> map;
    char dir = '^';
    Position position;
    std::size_t rows = 0;
    std::size_t columns = 0;
    while (getline( input, line))
    {
        columns = std::max( columns, line.size());
        map.push_back( line);
        if (const auto col = line.find(dir); col != line.npos)
        {
            position = {rows, col};
        }
        ++rows;
    }

    // walk the guard
    Guard g{position};
    while (g.position.col < columns and g.position.row < rows)
    {
        g.Peek( g.position, map) = 'X';
        Position nextPos = g.Step( g.position);
        if (nextPos.col >= columns or nextPos.row >= rows) break;
        while (g.Peek( nextPos, map) == '#')
        {
            g.Rotate();
            nextPos = g.Step( g.position);
        }
        g.position = nextPos;
    }

    std::size_t sum = std::ranges::count( map | std::views::join , 'X');

    std::cout << "unique positions count: " << sum << '\n';
}


