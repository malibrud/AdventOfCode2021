#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct Test
{
    std::string input;
    std::string param;
    std::string output;
};

std::vector<std::string> readLines( std::string fileName );
std::vector<int> readInts( std::string fileName );
bool runTests( std::vector<Test> tests, std::string (*func)(std::string, std::string) );

void runPart(std::string part, std::string (*func)(std::string, std::string), std::vector<Test> &tests)
{
    printf("Running Part %s:\n", part.c_str());
    runTests(tests, func);

    auto answer = func("data.txt", "");
    printf("The answer is: %s\n", answer.c_str());
    printf("\n\n");
}

bool runTests( std::vector<Test> tests, std::string (*func)(std::string, std::string) )
{
    printf("Running tests...\n");
    bool allPass = true;
    for ( size_t i = 0; i < tests.size(); i++ )
    {
        Test &t = tests[i];
        printf("[%d] %s(%s) -> %s...   ", i, t.input.c_str(), t.param.c_str(), t.output.c_str());
        std::string result = func( t.input, t.param );
        if (result == t.output)
        {
            printf("Pass\n");
        }
        else
        {
            printf("Fail\n");
            allPass = false;
        }
    }
    return allPass;
}

std::vector<std::string> readLines( std::string fileName )
{
    std::vector<std::string> lines;

    std::ifstream f( fileName );
    if ( !f.is_open() ) return lines;

    std::string line;
    while ( getline( f, line ) )
    {
        lines.push_back( line );
    }
    f.close();
    return lines;
}

std::vector<int> readInts( std::string fileName )
{
    std::vector<std::string> lines = readLines( fileName );
    std::vector<int> v;
    for (auto line: lines)
    {
        auto val = stoi(line);
        v.push_back(val);
    }
    return v;
}