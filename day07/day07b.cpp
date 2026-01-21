#include <charconv>
#include <iostream>
#include <fstream>
#include <ranges>
#include <regex>
#include <span>
#include <string>

#include "../timer.h"

namespace {
    using Number = std::uint64_t;

    bool Solvable( const Number result, const Number soFar, std::span<std::string_view> elements)
    {
        if (soFar > result) return false;
        if (elements.empty()) return result == soFar;

        Number front{};
        std::from_chars( elements.front().begin(), elements.front().end(), front);

        const auto multiplied = soFar * front;
        const auto added = soFar + front;

        // try all combinations, but be aware of overflow.
        if (
            (multiplied >= soFar and Solvable( result, multiplied, elements.subspan<1>()))
            or (added   > soFar and Solvable( result, added, elements.subspan<1>())))
        {
            return true;
        }

        // no views::concat until C++26
        const auto concatenated = std::stoull( std::to_string( soFar) + std::string(elements.front()));
        return concatenated > soFar and Solvable( result, concatenated, elements.subspan<1>());
    }

    bool Solvable( const Number result, std::span<std::string_view> elements)
    {
        Number front;
        std::from_chars( elements.front().begin(), elements.front().end(), front);
        return Solvable( result, front, elements.subspan<1>());
    }
}

int main()
{
    Timer _;
    std::ifstream input{"input07.txt"};
    std::string line;

    const std::regex equationPattern{"(\\d+): (.*)"};
    std::smatch match;

    Number sum = 0;
    while (getline( input, line) and std::regex_match( line, match, equationPattern))
    {
        std::vector< std::string_view> numbers;
        const auto result = std::stoull( match[1]);
        for (const auto &numString : std::views::split( std::string_view{match[2].first, match[2].second}, ' '))
        {
            numbers.emplace_back( numString.begin(), numString.end());
        }

        if (Solvable( result, numbers)) sum += result;
    }

    std::cout << "sum: " << sum << '\n';
}
