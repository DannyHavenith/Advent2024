#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../timer.h"

namespace {
}

int main()
{
    Timer _;
    std::ifstream input{"input09.txt"};
    std::string line;
    getline( input, line);

    struct File
    {
        int id; // -1 is empty space
        std::uint8_t size = 0;
    };


    using Blocks = std::vector<File>;
    Blocks blocks;

    int counter = 0;

    auto it = line.begin();
    while(  it != line.end())
    {
        blocks.emplace_back( counter++, *it - '0');
        ++it;
        if (it != line.end())
        {
            blocks.emplace_back( -1, *it - '0');
            ++it;
        }
    }

    // # blocks is even, so it is > 0 as well...
    assert( blocks.size() % 2 == 1);

    std::array<unsigned int, 10> indexOfFirstSpaceOfSize({1});

    std::size_t sum = 0;
    for ( auto lastFileIndex = blocks.size() - 1; lastFileIndex; --lastFileIndex)
    {
        auto &lastFile = blocks[lastFileIndex];
        if (lastFile.id < 0) continue;

        auto &freeSpaceIndex = indexOfFirstSpaceOfSize[lastFile.size];
        while (freeSpaceIndex < lastFileIndex and (blocks[freeSpaceIndex].id != -1 or blocks[freeSpaceIndex].size < lastFile.size)) ++freeSpaceIndex;

        for (auto size = lastFile.size + 1; size < 10; ++size)
        {
            indexOfFirstSpaceOfSize[size] = std::max( indexOfFirstSpaceOfSize[size], freeSpaceIndex);
        }

        if ( freeSpaceIndex < lastFileIndex)
        {
            if (const auto remaining = blocks[freeSpaceIndex].size - lastFile.size; remaining > 0)
            {
                blocks.insert( blocks.begin() + lastFileIndex + 1, {-1, lastFile.size});
                std::rotate(blocks.begin() + freeSpaceIndex, blocks.begin() + lastFileIndex, blocks.begin() + lastFileIndex + 1);
                ++freeSpaceIndex;
                blocks[freeSpaceIndex].size = remaining;
            }
            else
            {
                std::swap( blocks[freeSpaceIndex], lastFile);
            }
        }
    }

    std::size_t position = 0;
    for (const auto &block : blocks)
    {
        if (block.id > 0)
        {
            sum += block.id * ((2 * position + block.size - 1) * block.size  / 2);
        }
        position += block.size;
    }

    std::cout << "checksum is: " << sum << '\n';
}
