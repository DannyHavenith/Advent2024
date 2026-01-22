#include <cassert>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>


#include "../timer.h"

namespace {
    using Map = std::vector<std::string>;
    using Position = std::pair< std::uint8_t, std::uint8_t>;
    using Positions = std::deque<Position>;
    using PositionToPathCount = std::map<Position, int>;
}

int main()
{
    Timer _;
    std::ifstream input{"input10.txt"};
    std::string line;

    Map map;
    Positions backlog;

    PositionToPathCount peaksAccessibleFrom;
    std::size_t rows = 0;

    // Read map, give each peak a unique number and register that (obviously) that
    // peak is accessible from its location.
    while (getline( input, line))
    {
        for (auto column = 0u; column < line.size(); ++column)
        {
            if (line[column] == '9')
            {
                peaksAccessibleFrom[{rows, column}] = 1;
            }
        }
        map.push_back(std::move( line));
        ++rows;
    }

    assert( not map.empty());
    const auto columns = map[0].size();

    for ( auto nextLevel = '8'; nextLevel >= '0'; --nextLevel)
    {
        // At this point:
        //
        // `peaksAccessibleFrom` contains the positions of all tiles of level
        // `nextLevel + 1` that have access to a peak and their corresponding
        // value is the set of peaks (unique peak ids) reachable from that tile.
        PositionToPathCount peaksAccessibleFromLowerLevel;
        for ( const auto &[pos,  pathCount] : peaksAccessibleFrom)
        {
            const auto &[row, column] = pos;

            // for this tile of level nextLevel + 1, search for neighbours of level nextLevel.
            for ( const auto &[peekRow, peekColumn] :
                {std::pair<std::uint8_t, std::uint8_t>{row-1, column},{row,column-1},{row,column + 1},{row+1, column}, })
            {
                if (peekRow < rows and peekColumn < columns and map[peekRow][peekColumn] == nextLevel)
                {
                    peaksAccessibleFromLowerLevel[{peekRow, peekColumn}] += pathCount;
                }
            }
        }
        peaksAccessibleFrom = std::move( peaksAccessibleFromLowerLevel);
    }

    std::size_t sum = 0;
    for (const auto &[ ignored, pathCount] : peaksAccessibleFrom)
    {
        sum += pathCount;
    }

    std::cout << "sum: " << sum << '\n';
}
