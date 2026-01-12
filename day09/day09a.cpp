#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <numeric>
#include <ranges>
#include <regex>
#include <string>
#include <vector>

#include "../timer.h"

namespace {
}

int main()
{
    Timer _;
    std::ifstream input{"input09.txt"};
    std::string blocks;
    getline( input, blocks);
    assert( line.size() % 2 == 1);

    // convert all blocks from one-digit characters ('0'-'9') to their numerical
    // value (0-9).
    for ( auto &block : blocks)
    {
        block -= '0';
    }

    auto frontBlock = blocks.begin();
    auto backBlock = std::prev( blocks.end());
    auto frontPos = 0u;
    auto backPos = std::accumulate( blocks.begin(), blocks.end(), 0u);

    std::size_t sum = 0;
    while (frontPos < backPos)
    {
        // frontBlock points to a file, not empty space
        {
            const auto fileId = std::distance( blocks.begin(), frontBlock) / 2;
            const auto fileSize = *frontBlock;
            sum += (fileSize * (2 * frontPos + fileSize - 1) / 2) * fileId;
            frontPos += fileSize;
        }

        if (frontPos >= backPos) break;

        ++frontBlock;
        while (*frontBlock)
        {
            // frontBlock and frontPos point to empty space, backBlock points to the last file
            const auto fillSize = std::min( *frontBlock, *backBlock);
            const auto backId = std::distance( blocks.begin(), backBlock) / 2;
            sum += (fillSize * (2 * frontPos + fillSize - 1) / 2) * backId;
            *frontBlock -= fillSize;
            *backBlock -= fillSize;

            frontPos += fillSize;
            backPos -= fillSize;

            while (not *backBlock)
            {
                // jump over empty space to the next file
                --backBlock;
                backPos -= *backBlock;
                --backBlock;
            }
        }

        ++frontBlock;
    }

    std::cout << "checksum is: " << sum << '\n';
}
