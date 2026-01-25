#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>

#include "../timer.h"

namespace {
    using Number = std::uint64_t;

    std::tuple< bool, Number, Number> SplitDigits( Number value)
    {
        Number left = value;
        Number right = 0;
        Number multiplier = 1;
        while (value)
        {
            // transfer digit from left to right
            right += multiplier * (left % 10);
            left /= 10;
            if (not (value /= 10)) return {false, left, right};
            multiplier *= 10;
            value /= 10;
        }

        return {true, left, right};
    }

    Number StoneCount( std::uint8_t blinks, Number value)
    {
        std::size_t newStones = 0;
        while (blinks)
        {
            --blinks;
            if (value == 0)
            {
                ++value;
            }
            else if (const auto &[success, left, right] = SplitDigits( value); success)
            {
            value = left;
                newStones += StoneCount( blinks, right);
            }
            else
            {
                value *= 2024;
            }
        }

        return 1 + newStones;
    }
}

int main()
{
    Timer _;
    std::ifstream input{"input11.txt"};
    std::string line;

    std::size_t sum = 0;
    while (getline( input, line))
    {
        std::stringstream ss{line};
        Number number;
        while ( ss >> number)
        {
            sum += StoneCount(25, number);
        }
    }

    std::cout << "sum: " << sum << '\n';
}
