#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_set>
#include <string>

#include "../timer.h"

namespace {
    using PageNumber = int;

    // page numbers are all < 100
    constexpr auto maxPages = 99;
    using AdjacencyList = std::array<std::vector<PageNumber>, maxPages>;

}

int main()
{
    Timer _;

    std::ifstream input{"input05.txt"};
    std::string line;

    const std::regex orderPattern{"(\\d+)\\|(\\d+)"};
    std::smatch match;
    AdjacencyList predecessors;

    while (getline( input, line) and std::regex_match( line, match, orderPattern))
    {
        predecessors[stoi(match[2])].push_back( stoi(match[1]));
    }

    std::size_t sum = 0;
    while (getline( input, line))
    {
        std::stringstream numberStream{line};
        std::string numberString;
        std::unordered_set<int> forbidden;
        std::vector<int> numbers;
        bool valid = true;
        while (getline( numberStream, numberString, ','))
        {
            const auto number = std::stoi( numberString);
            if (forbidden.contains( number))
            {
                valid = false;
                break;
            }
            const auto &newForbidden = predecessors[number];
            forbidden.insert( newForbidden.begin(), newForbidden.end());
            numbers.push_back( number);
        }
        if (valid)
        {
            sum += numbers[numbers.size()/2];
        }
    }

    std::cout << "sum of middle page numbers: " << sum << '\n';
}
