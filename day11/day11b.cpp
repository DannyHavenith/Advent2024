#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>

#include "../timer.h"

namespace {
    using Number = std::uint64_t;

    struct PairHash
    {
    public:
        template <typename T, typename U>
        std::size_t operator()(const std::pair<T, U> &x) const
        {
            return std::hash<T>()(x.first) + std::hash<U>()(x.second);
        }
    };

    using CacheType = std::unordered_map< std::pair< std::uint8_t, Number>, Number, PairHash>;

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

    Number StoneCount( std::uint8_t blinks, Number value, CacheType &cache)
    {
        if (not blinks) return 1;

        const auto it = cache.find( {blinks, value});
        if ( it != cache.end()) return it->second;

        if (value == 0)
        {
            return cache[{blinks, value}] = StoneCount( blinks - 1, value + 1, cache);
        }

        if (const auto &[success, left, right] = SplitDigits( value); success)
        {
            return cache[{blinks, value}] =
                StoneCount( blinks - 1, left, cache) + StoneCount( blinks - 1, right, cache);
        }
        else
        {
            return cache[{blinks, value}] = StoneCount( blinks - 1, value * 2024, cache);
        }
    }
}

int main()
{
    Timer _;
    std::ifstream input{"input11.txt"};
    std::string line;

    CacheType cache;

    std::size_t sum = 0;
    while (getline( input, line))
    {
        std::stringstream ss{line};
        Number number;
        while ( ss >> number)
        {
            sum += StoneCount(75, number, cache);
        }
    }

    std::cout << "sum: " << sum << '\n';
}
