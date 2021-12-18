#include <vector>
#include <map>
#include <algorithm>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;


vector<Test> partATests =
{
    {"test.txt", "", "26397"}
};
string paramA = ""; 
 
vector<Test> partBTests =
{
    {"test.txt", "", "288957"} 
};
string paramB = "";

string computePartA( string fileName, string param )
{
    string openers = "([{<";
    string closers = ")]}>";

    map<char, int> values
    {
        {')', 3},
        {']', 57},
        {'}', 1197},
        {'>', 25137}
    };

    map<char, char> pairs
    {
        { '(', ')' },
        { '[', ']' },
        { '{', '}' },
        { '<', '>' }
    };

    auto lines = readLines(fileName);

    vector<char> nesting;
    int sum = 0;
    for (auto &line : lines)
    {
        nesting.clear();
        for (auto i = line.begin() ; i != line.end() ; i++)
        {
           if ( openers.find( *i ) != string::npos )
           {
               nesting.push_back( *i );
           }
           else if ( closers.find( *i ) != string::npos )
           {
               if ( *i != pairs[ nesting.back() ] ) 
               {
                    sum += values[ *i ];
                    break;
               }
               nesting.pop_back();
           }
        }
        
    }

    return to_string(sum);
}

string computePartB( string fileName, string param )
{
    string openers = "([{<";
    string closers = ")]}>";

    map<char, int> values
    {
        {')', 1},
        {']', 2},
        {'}', 3},
        {'>', 4}
    };

    map<char, char> pairs
    {
        { '(', ')' },
        { '[', ']' },
        { '{', '}' },
        { '<', '>' }
    };

    auto lines = readLines(fileName);

    vector<long long> scores;
    vector<char> nesting;
    int sum = 0;
    for (auto &line : lines)
    {
        nesting.clear();
        bool corrupted = false;
        for (auto i = line.begin() ; i != line.end() ; i++)
        {
           if ( openers.find( *i ) != string::npos )
           {
               nesting.push_back( *i );
           }
           else if ( closers.find( *i ) != string::npos )
           {
               if ( *i != pairs[ nesting.back() ] )  
               {
                   corrupted = true;
                   break;
               };
               nesting.pop_back();
           }
        }
        if (corrupted) continue;

        long long score = 0;
        while (nesting.size() > 0)
        {
            char opener = nesting.back();
            char closer = pairs[ opener];
            int points = values[ closer];
            score = score * 5 + points;
            nesting.pop_back();
        }
        scores.push_back(score);
    }
    auto N = scores.size();
    sort(scores.begin(), scores.end());
    auto midVal = scores[N/2];

    return to_string(midVal);
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
