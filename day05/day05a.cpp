#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <set>
#include <string>

#include "../timer.h"

namespace {
    using AdjacencyList = std::map<int, std::set<int>>;
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
        predecessors[stoi(match[2])].insert( stoi(match[1]));
    }

    std::size_t sum = 0;
    while (getline( input, line))
    {
        std::stringstream numberStream{line};
        std::string numberString;
        std::set<int> forbidden;
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
