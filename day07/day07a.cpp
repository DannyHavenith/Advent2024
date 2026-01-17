#include <iostream>
#include <fstream>
#include <ranges>
#include <regex>
#include <span>
#include <string>

#include "../timer.h"

namespace {
    using Number = std::uint64_t;


    bool Solvable( const Number result, const Number soFar, std::span<Number> elements)
    {
        if (soFar > result) return false;
        if (elements.empty()) return result == soFar;

        const auto multiplied = soFar * elements.front();
        const auto added = soFar + elements.front();

        // try all combinations, but be aware of overflow.
        return
            (multiplied >= soFar and Solvable( result, multiplied, elements.subspan<1>()))
            or (added   > soFar and Solvable( result, added, elements.subspan<1>()));
    }

    bool Solvable( const Number result, std::span<Number> elements)
    {
        return Solvable( result, elements.front(), elements.subspan<1>());
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
        std::vector< Number> numbers;
        const auto result = std::stoull( match[1]);
        for (const auto &numString : std::views::split( std::string_view{match[2].first, match[2].second}, ' '))
        {
            numbers.push_back( std::stoull( std::string{numString.begin(), numString.end()}));
        }

        if (Solvable( result, numbers)) sum += result;
    }

    std::cout << "sum: " << sum << '\n';
}
