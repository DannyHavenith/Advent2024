#include <fstream>
#include <iostream>
#include <set>
#include <ranges>
#include <regex>

#include "../timer.h"


using Number = int;

int main()
{
    Timer _;
    namespace ranges = std::ranges;
    std::ifstream input("input01.txt");

    std::string line;
    std::regex numbers{"(\\d+)\\s+(\\d+)"};
    std::smatch match;

    std::multiset<Number> left;
    std::multiset<Number> right;

    while (std::getline(input, line) and std::regex_match(line, match, numbers))
    {
        left.insert(std::stoi(match.str(1)));
        right.insert(std::stoi(match.str(2)));
    }

    int sum = 0;
    for (const auto value : left)
    {
        sum += value * right.count( value);
    }

    std::cout << "sum of products: " << sum << '\n';

    return 0;
}
