#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <ranges>
#include <regex>
#include <string>
#include <unordered_set>
#include <set>
#include "../timer.h"

namespace {
    using PageNumber = int;

    // page numbers are all < 100
    constexpr auto maxPages = 99;
    using AdjacencyList = std::array<std::vector<PageNumber>, maxPages>;
    using AdjacencyMap = std::map< PageNumber, std::vector<PageNumber>>;
    using NodeFlags = std::array< bool, maxPages>;
    using NodeOrder = std::map<PageNumber, int>;

     /**
     * Sort the nodes of the graph topologically. Or rather, rank each node
     * topologically in the NodeOrder parameter
     *
     */
    void TopologicalSort( const AdjacencyMap &graph, PageNumber start, NodeFlags &visited, NodeOrder &order)
    {
        static auto nodeOrder = 1;
        if (visited[start]) return;

        visited[start] = true;
        if (const auto successor = graph.find( start); successor != graph.end())
        {
            for ( const auto child : successor->second)
            {
                TopologicalSort( graph, child, visited, order);
            }
        }

        order[start] = nodeOrder++;
    }

    NodeOrder TopologicalSort( const AdjacencyMap &graph, std::set<PageNumber> startingNodes)
    {
        NodeFlags visited{};
        NodeOrder nodeOrder{};
        for ( const auto node : startingNodes)
        {
            TopologicalSort( graph, node, visited, nodeOrder);
        }

        return nodeOrder;
    }

    void Fix(
        std::vector<PageNumber> &numbers,
        AdjacencyList &predecessorsOf, const std::set<PageNumber> &)
    {
        using namespace std::views;
        std::set<PageNumber> pagesSet{ numbers.begin(), numbers.end()};
        std::set<PageNumber> pagesWithoutSuccessors = pagesSet;
        AdjacencyMap limitedAdjacencies;
        for ( const auto number : numbers)
        {
            for ( const auto predecessor : predecessorsOf[number])
            {
                if (pagesSet.contains( predecessor))
                {
                    limitedAdjacencies[number].push_back( predecessor);
                    pagesWithoutSuccessors.erase( predecessor);
                }
            }
        }
        auto pageOrder = TopologicalSort( limitedAdjacencies, pagesWithoutSuccessors);
        std::ranges::stable_sort(
            numbers,
            [&pageOrder](PageNumber left, PageNumber right)
            {
                return pageOrder[left] and pageOrder[left] < pageOrder[right];
            });
    }

    std::vector<PageNumber> StringToNumbers( const std::string &line)
    {
        std::stringstream numberStream{line};
        std::string numberString;
        std::vector<PageNumber> numbers;
        while (getline( numberStream, numberString, ','))
        {
            const auto number = std::stoi( numberString);
            numbers.push_back( number);
        }
        return numbers;
    }

    bool IsValid( const std::vector<PageNumber> &numbers, const AdjacencyList &predecessorsOf)
    {
        std::unordered_set<PageNumber> forbidden;

        for ( const auto number : numbers)
        {
            if (forbidden.contains( number))
            {
                return false;
            }

            const auto &newForbidden = predecessorsOf[number];
            forbidden.insert( newForbidden.begin(), newForbidden.end());
        }

        return true;
    }
}

int main()
{
    Timer _;

    std::ifstream input{"input05.txt"};
    std::string line;

    const std::regex orderPattern{"(\\d+)\\|(\\d+)"};
    std::smatch match;
    AdjacencyList predecessorsOf;

    std::set<PageNumber> predecessors;
    while (getline( input, line) and std::regex_match( line, match, orderPattern))
    {
        const auto predecessor = stoi(match[1]);
        predecessorsOf[stoi(match[2])].push_back( predecessor);
        predecessors.insert( predecessor);
    }

    std::size_t sum = 0;
    while (getline( input, line))
    {
        auto numbers = StringToNumbers( line);

        if (not IsValid( numbers, predecessorsOf))
        {
            Fix( numbers, predecessorsOf, predecessors);
            assert( IsValid( numbers, predecessorsOf));
            sum += numbers[numbers.size()/2];
        }
    }

    std::cout << "sum of middle page numbers: " << sum << '\n';
}
