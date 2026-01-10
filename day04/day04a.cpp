#include <iostream>
#include <fstream>
#include <regex>
#include <string>

#include "../timer.h"

namespace {
    struct WordFinder
    {
        std::string_view word;
        size_t position;
        std::int8_t offset;

        std::pair< bool, std::size_t> NextLine( const std::string &line)
        {
            position += offset;

            // if there's a mismatch, return 0 and signal we can be deleted
            if (position >= line.size() or line[position] != word.front()) return {true, 0};
            word.remove_prefix(1);

            // if we fully matched, return 1 and signal we can be deleted
            if (word.empty()) return {true, 1};

            // more matching to do, return 0 and signal we can't be deleted yet.
            return {false, 0};

        }
    };
}

int main()
{
    Timer _;
    std::ifstream input{"input04.txt"};
    std::string line;
    std::vector<WordFinder> wordFinders;

    std::size_t count = 0;
    while (std::getline( input, line))
    {
        // search for horizontal matches
        auto i = 0u;
        while ( i < line.size() - 3)
        {
            if (
                (line[i] == 'X' and line[i+1] == 'M' and line[i+2] == 'A' and line [i+3] == 'S') or
                (line[i] == 'S' and line[i+1] == 'A' and line[i+2] == 'M' and line [i+3] == 'X'))
            {
                ++count;
                i += 3;
            }
            else
            {
                ++i;
            }
        }

        // Let the word finders search for matches
        auto finder = wordFinders.begin();
        while (finder != wordFinders.end())
        {
            const auto [doDelete, wordFound] = finder->NextLine( line);
            count += wordFound;
            if (doDelete)
            {
                finder = wordFinders.erase( finder);
            }
            else
            {
                ++finder;
            }
        }

        // create new word finders
        for (auto pos = 0u; pos < line.size(); ++ pos)
        {
            using namespace std::literals;
            static const auto xmasContinuation = "MAS"s;
            static const auto samxContinuation = "AMX"s;
            std::string_view word;
            if (line[pos] == 'X')
            {
                word = xmasContinuation;
            }
            else if (line[pos] == 'S')
            {
                word = samxContinuation;
            }

            if (not word.empty())
            {
                wordFinders.emplace_back( word, pos, -1);
                wordFinders.emplace_back( word, pos, 0);
                wordFinders.emplace_back( word, pos, 1);
            }
        }
    }

    std::cout << "count: " << count << '\n';
    return 0;
}
