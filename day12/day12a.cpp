#include <cassert>
#include <cctype>
#include <deque>
#include <iostream>
#include <fstream>
#include <ranges>
#include <string>
#include <vector>
#include "../timer.h"

namespace {
    using Map = std::vector<std::string>;
    using Number = std::uint8_t;

    std::size_t AreaCost( Map &map, std::size_t row, std::size_t column, std::size_t rows, std::size_t columns)
    {
        const auto marker = map[row][column];
        const auto replacement = std::tolower( marker);
        if (marker == replacement) return 0;

        std::deque< std::pair<Number, Number>> backlog{{row, column}};
        std::size_t area = 0;
        std::size_t perimeter = 0;

        while (not backlog.empty())
        {
            const auto [centerRow, centerColumn] = backlog.front();
            backlog.pop_front();

            if (map[centerRow][centerColumn] != marker) continue; // we've been here already
            map[centerRow][centerColumn] = replacement;
            ++area;

            for (const auto &[r,c] : {
                std::pair<Number, Number>
                    { centerRow - 1, centerColumn},
                    {centerRow, centerColumn - 1},
                    {centerRow, centerColumn + 1},
                    {centerRow + 1, centerColumn}})
            {
                if ( c >= columns or r >= rows)
                {
                    ++perimeter;
                }
                else
                {
                    if (map[r][c] == marker)
                    {
                        backlog.emplace_back(r, c);
                    }
                    else if (map[r][c] != replacement)
                    {
                        ++perimeter;
                    }
                }
            }
        }
        return area * perimeter;
    }
}



int main()
{
    Timer _;
    std::ifstream input{"input12.txt"};
    std::string line;

    Map map;
    while (getline( input, line))
    {
        map.push_back( std::move(line));
    }
    assert( not map.empty());

    const auto rows = map.size();
    const auto columns = map[0].size();

    std::size_t sum = 0;
    for ( const auto [row, column] : std::views::cartesian_product( std::views::iota(0u, rows), std::views::iota(0u,columns)))
    {
        sum += AreaCost( map, row, column, rows, columns);
    }

    std::cout << "sum: " << sum << '\n';
}
