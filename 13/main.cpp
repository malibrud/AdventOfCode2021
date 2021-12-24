#include <list>
#include <algorithm>
#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;

vector<Test> partATests =
{
    {"test1.txt", "", "17"}
};
string paramA = ""; 
 
vector<Test> partBTests =
{
    {"test1.txt", "", ""}
};
string paramB = "";

string computePartA( string fileName, string param )
{
    list< pair<int, int> > dots;
    vector< pair<char, int> > folds;

    auto lines = readLines(fileName);

    bool parsingCoords = true;
    bool parsingFolds = false;

    for (auto &line: lines)
    {
        // Sense the empty line which separates dots from folds.
        if ( line =="" )
        {
            parsingCoords = false;
            parsingFolds = true;
            continue;
        }
        
        // Read the dot coordinates
        if ( parsingCoords )
        {
            auto fields = splitString( line, ',' );
            auto x = stoi( fields[0] );
            auto y = stoi( fields[1] );
            pair<int, int> dot(x, y);
            dots.push_back(dot);
        }

        // Read the folds
        if ( parsingFolds )
        {
            vector<string> fields;
            fields = splitString( line, ' ' );
            fields = splitString( fields[2], '=' );
            auto axis = fields[0][0];
            auto value = stoi( fields[1] );
            pair<char, int> fold( axis, value );
            folds.push_back(fold);
        }
    }

    // Fold once
    auto fold = folds[0];
    for (auto &dot: dots)
    {
        auto axis = fold.first;
        auto value = fold.second;
        if ( axis == 'x' && dot.first  > value ) 
        {
            dot.first = 2*value  - dot.first;
        }
        else if ( axis == 'y' && dot.second > value )
        {
            dot.second = 2*value - dot.second;
        }
    }

    // Remove the duplicate dots.
    dots.sort();
    dots.unique();

    auto count = dots.size();

    return to_string(count);
}

string computePartB( string fileName, string param )
{
    return computePartA( fileName, param );
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
