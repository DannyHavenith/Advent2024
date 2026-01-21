#include <iostream>
#include <fstream>
#include <regex>
#include <string>

#include "../timer.h"

namespace {
}

int main()
{
    Timer _;
    std::ifstream input{"input.txt"};
    std::string line;

    const std::regex somePattern{"(\\d+)\\|(\\d+)"};
    std::smatch match;


    std::size_t sum = 0;
    while (getline( input, line) and std::regex_match( line, match, somePattern))
    {
    }

    std::cout << "sum: " << sum << '\n';
}
