#include <iostream>
#include <fstream>
#include <regex>

#include "../timer.h"

int main()
{
    Timer _;
    std::ifstream input{"input03.txt"};
    std::string memory{ std::istreambuf_iterator<char>{input}, {}};
    std::regex instruction{R"(mul\((\d+),(\d+)\))"};
    using Tokenizer = std::sregex_token_iterator;

    std::size_t sum = 0;
    for ( auto it = Tokenizer{memory.begin(), memory.end(), instruction, {1,2}}; it != Tokenizer{}; ++it)
    {
        size_t mult = stoul( *it++);
        mult *= std::stoul( *it);
        sum += mult;
    }

    std::cout << "sum of mults is: " << sum << '\n';

}
