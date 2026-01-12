#include <algorithm>
#include <iostream>
#include <fstream>
#include <regex>
#include <set>
#include <string>

#include "../timer.h"

namespace {
    using Position = std::pair<unsigned int, unsigned int>;
    using Positions = std::set<Position>;
    using Antennas = std::map< char, Positions>;

    Position Extend( const Position &a, const Position &b)
    {
        return { b.first + (b.first - a.first), b.second + (b.second - a.second)};
    }
}

int main()
{
    Timer _;
    std::ifstream input{"input08.txt"};
    std::string line;

    auto rows = 0u;
    std::size_t columns = 0;
    Antennas antennas;
    while (getline( input, line))
    {
        columns = std::max( columns, line.size());
        for (auto col = 0u; col < line.size(); ++col)
        {
            if (std::isalnum(line[col])) antennas[line[col]].emplace( rows, col);
        }
        ++rows;
    }

    Positions antinodes;
    // unfortunately, there's no half_cartesian_product view...
    for (const auto &[_ignore, positions] : antennas)
    {
        for (auto first = positions.begin(); first != positions.end(); ++first)
        {
            for (auto second = std::next(first); second != positions.end(); ++second)
            {
                for ( const auto &[firstAntenna, secondAntenna] : {std::pair{*first, *second}, {*second, *first}})
                {
                    const auto antinode = Extend( firstAntenna, secondAntenna);
                    if (antinode.first < rows and antinode.second < columns)
                    {
                        antinodes.insert( antinode);
                    }
                }
            }
        }
    }

    std::cout << "unique antinodes: " << antinodes.size() << '\n';
}
