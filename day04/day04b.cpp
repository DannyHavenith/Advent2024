#include <cassert>
#include <deque>
#include <iostream>
#include <fstream>
#include <regex>
#include <string>

#include "../timer.h"


int main()
{
    Timer _;
    std::ifstream input{"input04.txt"};
    std::string line;

    // keep a 3-line window on the input file
    std::deque<std::string> window;

    if (not getline( input, line)) return 1;
    window.push_back( line);
    if (not getline( input, line)) return 1;
    window.push_back( line);


    std::size_t count = 0;
    while (std::getline( input, line))
    {
        const auto &prev = window[0];
        const auto &current = window[1];
        const auto &next = line;

        assert( next.size() == current.size());
        assert( prev.size() == current.size());

        for ( auto i = 1u; i < current.size() - 1; ++i)
        {
            if (
                current[i] == 'A' and
                ((prev[i-1] == 'M' and next[i+1] == 'S') or (prev[i-1] == 'S' and next[i+1] == 'M')) and
                ((prev[i+1] == 'M' and next[i-1] == 'S') or (prev[i+1] == 'S' and next[i-1] == 'M'))
            ) ++count;
        }

        window.pop_front();
        window.push_back( line);
    }

    std::cout << "count: " << count << '\n';
    return 0;
}
