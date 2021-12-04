#pragma once

#include <string>
#include <vector>

std::vector<std::string> splitString(std::string str, const char delim)
{
    auto len = str.length();

    std::vector<std::string> output;
    
    auto curPos = 0;
    while (curPos < len)
    {
        auto didx = str.find(delim, curPos);
        if ( didx == std::string::npos ) didx = len;
        auto field = str.substr(curPos, didx - curPos);
        output.push_back(field);
        curPos = didx + 1;
    }
    return output;
}

std::vector<int> parseCsvInts(std::string str)
{
    auto fields = splitString(str, ',');
    std::vector<int> vals;
    vals.reserve(fields.size());
    for (auto f : fields)
    {
        vals.push_back(std::stoi(f));
    }
    return vals;
}