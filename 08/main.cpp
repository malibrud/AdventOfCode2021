#include <regex>
#include <map>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;


vector<Test> partATests =
{
    {"test.txt", "", "26"}
};
string paramA = ""; 
 
vector<Test> partBTests =
{
    {"test.txt", "", "61229"} 
};
string paramB = "";

string computePartA( string fileName, string param )
{
    auto lines = readLines(fileName);
    regex pattern(".*\\| ([a-z]+) ([a-z]+) ([a-z]+) ([a-z]+)");
    auto count = 0;
    for ( auto line : lines )
    {
        smatch match;
        regex_match(line, match, pattern);
        for (int i = 1 ; i <= 4 ; i++)
        {
            auto len = match[i].length();
            if      (len == 2) count++; // 1
            else if (len == 3) count++; // 7
            else if (len == 4) count++; // 4
            else if (len == 7) count++; // 8
        }
    }
    return to_string(count);
}

pair<const string, int> getElementOfLength(map<const string, int> &m, int len);
int countCommonChars(const string s1, const string s2);
string diff(string s1, string s2);
string intersect(string s1, string s2);

string computePartB( string fileName, string param )
{
    auto lines = readLines(fileName);
    regex inputPattern("([a-z]+) ([a-z]+) ([a-z]+) ([a-z]+) ([a-z]+) ([a-z]+) ([a-z]+) ([a-z]+) ([a-z]+) ([a-z]+)");
    regex outputPattern(".*\\| ([a-z]+) ([a-z]+) ([a-z]+) ([a-z]+)");
    int sum = 0;
    for ( auto line : lines )
    {
        // Read and parse the input
        smatch inMatch;
        auto found = regex_search(line, inMatch, inputPattern);
        map<const string, int> input;
        for (int i = 0 ; i < 10 ; i++)
        {
            string val = inMatch[i+1];
            sort(val.begin(), val.end());
            auto len = val.length();
            input[val] = -1;
        }

        char segTop, segMid, segBot, segUL, segUR, segLL, segLR;

        // Decode the 7-segment displays
        // Find 1
        auto e = getElementOfLength(input, 2);
        auto one = e.first;
        input[one] = 1;

        // Find 4
        auto e4 = getElementOfLength(input, 4);
        auto four = e4.first;
        input[four] = 4;

        // Find 7
        auto e7 = getElementOfLength(input, 3);
        auto seven = e7.first;
        input[seven] = 7;

        if (one.find(seven[0]) == string::npos) segTop = seven[0];
        if (one.find(seven[1]) == string::npos) segTop = seven[1];
        if (one.find(seven[2]) == string::npos) segTop = seven[2];

        // Find 8
        auto e8 = getElementOfLength(input, 7);
        auto eight = e8.first;
        input[eight] = 8;

        // Find 9 - Should have 6 segments and be different from four + top by one.
        string nine;
        for (auto e : input)
        {
            if (e.second != -1) continue;

            auto lengthIs6 = (e.first.length() == 6);
            auto bottom = diff(e.first, four + segTop);
            if (lengthIs6 && bottom.length() == 1) 
            {
                nine = e.first;
                input[nine] = 9;
                segBot = bottom[0];
            };
        }
        
        // Find 6 - Should have length of 6 and not be zero.
        string six;
        for (auto e : input)
        {
            if (e.second != -1) continue;

            auto lengthIs6 = (e.first.length() == 6);

            if (!lengthIs6) continue;

            auto bottomRight = intersect(e.first, one);
            if (lengthIs6 && bottomRight.length() == 1) 
            {
                six = e.first;
                input[six] = 6;
                segLR = bottomRight[0];
                segUR = diff(one, bottomRight)[0];
            };
        }

        // Find 0 - Should be the only one left with only 6 segments
        string zero;
        for (auto e : input)
        {
            if (e.second != -1) continue;

            auto lengthIs6 = (e.first.length() == 6);
            if (lengthIs6) 
            {
                zero = e.first;
                input[zero] = 0;
            };
        }
        segMid = diff(eight, zero)[0];
        
        // Find 5 - Should be length of 5 and be entirely contained in 6.
        string five;
        for (auto e : input)
        {
            if (e.second != -1) continue;

            auto lengthIs5   = (e.first.length() == 5);
            auto commonWith6 = (countCommonChars(e.first, six) == 5);
            if (lengthIs5 && commonWith6) 
            {
                five = e.first;
                input[five] = 5;
            };
        }
        segLL = diff(five, six)[0];

        // Find 3 - Should be length of 5 and have four segments in common with 4.
        string three;
        char threeChars[] = {segTop, segMid, segBot, segUR, segLR, '\0'};
        string threePatern(threeChars);
        for (auto e : input)
        {
            if (e.second != -1) continue;
            if (diff(e.first, threePatern) == "") 
            {
                three = e.first;
                input[three] = 3;
            };
        }

        // Find 2 - Should be the only string not found.
        string two;
        for (auto e : input)
        {
            if (e.second != -1) continue;

            two = e.first;
            input[two] = 2;
        }

        // Now read the actual values
        smatch outMatch;
        regex_search(line, outMatch, outputPattern);

        int place = 1000;
        int val = 0;
        for (int i = 1 ; i <= 4 ; i++)
        {
            string key = outMatch[i];
            sort(key.begin(), key.end());
            val += input[key] * place;
            place /= 10;
        }
        sum += val;
    }
    return to_string(sum);
}

pair<const string, int> getElementOfLength(map<const string, int> &m, int len)
{
    for (auto e : m)
    {
        if (e.first.length() == len) return e;
    }
}

int countCommonChars(const string s1, const string s2)
{
    auto count = 0;
    for (auto c : s1)
    {
        if (s2.find(c) != string::npos) count++;
    }
    return count;
}

string diff(string s1, string s2)
{
    string result;
    for (auto c : s1)
    {
        if (s2.find(c) == string::npos) result.push_back(c);
    }
    return result;
}

string intersect(string s1, string s2)
{
    string result;
    for (auto c : s1)
    {
        if (s2.find(c) != string::npos) result.push_back(c);
    }
    return result;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        runPart("A", computePartA, partATests, paramA);
        runPart("B", computePartB, partBTests, paramB);
    }
    else if (string(argv[1]) == "A")
    {
        runPart("A", computePartA, partATests, paramA);
    }
    else if (string(argv[1]) == "B")
    {
        runPart("B", computePartB, partBTests, paramB);
    }
}
