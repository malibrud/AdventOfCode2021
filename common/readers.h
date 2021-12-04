#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>

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

std::list<std::string> readLinesAsList( std::string fileName )
{
    std::list<std::string> lines;

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