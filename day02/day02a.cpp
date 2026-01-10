#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <string>

#include "../timer.h"

namespace {
    using Tokenizer = std::sregex_token_iterator;

    template<typename Range>
    bool IsSafe( const Range &r)
    {
        auto currentIterator = r.begin();
        auto previous = *currentIterator++;
        auto previousDiff = *currentIterator - previous;

        for (;currentIterator != r.end(); ++currentIterator)
        {
            auto diff = *currentIterator - previous;
            if (previousDiff * diff <= 0) return false;
            if (std::abs(diff) > 3) return false;
            previousDiff = diff;
            previous = *currentIterator;
        }

        return true;
    }
}

int main()
{
    Timer _;
    std::ifstream input("input02.txt");

    std::string line;
    std::regex number{"(\\d+)"};
    std::size_t counter = 0;
    while (std::getline( input, line))
    {
        using namespace std::ranges;
        using namespace std::views;

        const auto numbers =
            subrange( Tokenizer( line.begin(), line.end(), number, 1), Tokenizer{})
            | transform( [](const auto &string){return std::stoi( string);});


        if (IsSafe( numbers))
        {
            ++counter;
        }
    }

    std::cout << "Safe range count: " << counter << '\n';
}
