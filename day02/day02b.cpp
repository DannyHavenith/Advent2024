#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <string>

#include "../timer.h"

namespace {

    using Tokenizer = std::sregex_token_iterator;

    template<typename Range, typename Iterator>
    bool IsSafe( const Range &range, const Iterator skip)
    {
        auto currentIterator = range.begin();
        if (currentIterator == skip) ++currentIterator;

        auto previous = *currentIterator++;
        if (currentIterator == skip) ++currentIterator;

        auto previousDiff = *currentIterator - previous;
        for (;currentIterator != range.end(); ++currentIterator)
        {
            if (currentIterator == skip) continue;

            auto diff = *currentIterator - previous;
            if (previousDiff * diff <= 0) return false;
            if (std::abs(diff) > 3) return false;
            previousDiff = diff;
            previous = *currentIterator;
        }

        return true;
    }

    template< typename T, typename Range>
    T To( const Range &r)
    {
        return T{r.begin(), r.end()};
    }

    bool IsSafeDampened(const std::vector<int> &range)
    {
        if (IsSafe( range, range.end())) return true;

        for ( auto skip = range.begin(); skip != range.end(); ++skip)
        {
            if (IsSafe( range, skip)) return true;
        }

        return false;
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

        auto numbersVector = To< std::vector<int>>( numbers);

        if (IsSafeDampened( numbersVector))
        {
            ++counter;
        }
    }

    std::cout << "Safe range count: " << counter << '\n';
}
