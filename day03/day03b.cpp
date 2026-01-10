#include <iostream>
#include <fstream>
#include <regex>

#include "../timer.h"

int main()
{
    Timer _;
    std::ifstream input{"input03.txt"};
    std::string memory{ std::istreambuf_iterator<char>{input}, {}};
    std::regex instructionPattern{R"((do\(\)|don't\(\)|mul\((\d+),(\d+)\)))"};
    using Tokenizer = std::sregex_token_iterator;

    std::size_t sum = 0;
    bool enabled = true;
    for ( auto it = Tokenizer{memory.begin(), memory.end(), instructionPattern, {1,2,3}}; it != Tokenizer{}; ++it)
    {
        std::string instruction = *it++;
        if (instruction == "do()")
        {
            advance( it, 1);
            enabled = true;
        }
        else if (instruction == "don't()")
        {
            advance( it, 1);
            enabled = false;
        }
        else if (enabled)
        {
            size_t mult = stoul( *it++);
            mult *= std::stoul( *it);
            sum += mult;
        }
        else
        {
            advance( it, 1);
        }
    }

    std::cout << "sum of mults is: " << sum << '\n';
}
