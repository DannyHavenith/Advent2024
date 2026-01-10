#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <regex>
#include <vector>

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

    std::vector<Number> left;
    std::vector<Number> right;

    while (std::getline(input, line) and std::regex_match(line, match, numbers))
    {
        left.push_back(std::stoi(match.str(1)));
        right.push_back(std::stoi(match.str(2)));
    }

    ranges::sort(left);
    ranges::sort(right);

    auto differences = std::views::zip(left, right) | std::views::transform(
        [](auto pair) {
            return std::abs(std::get<0>(pair) - std::get<1>(pair));
        });

    auto total_difference = std::accumulate( differences.begin(), differences.end(), 0);

    std::cout << "sum of differences: " << total_difference << '\n';

    return 0;
}
