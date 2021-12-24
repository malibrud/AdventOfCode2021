#include <list>
#include <cmath>
#include <algorithm>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;

vector<Test> partATests =
{
    {"test1.txt", "1", "17"}
};
string paramA = "1"; 
 
vector<Test> partBTests =
{
    {"test1.txt", "2", "16"}
};
string paramB = "12";

string computePartA( string fileName, string param )
{
    list< pair<int, int> > dots;
    vector< pair<char, int> > folds;

    auto maxFolds = stoi( param );
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

    // Fold the requested number of times.
    auto nFolds = min( maxFolds, (int)folds.size() );
    for ( int i = 0 ; i < nFolds ; i++ )
    {
        auto &fold = folds[i];
        auto axis = fold.first;
        auto value = fold.second;
        for (auto &dot: dots)
        {
            if ( axis == 'x' && dot.first  > value ) 
            {
                dot.first = 2*value  - dot.first;
            }
            else if ( axis == 'y' && dot.second > value )
            {
                dot.second = 2*value - dot.second;
            }
        }
    }

    // Remove the duplicate dots.
    dots.sort();
    dots.unique();

    // Print the characters only if all folds are done.
    if ( maxFolds == (int)folds.size() )
    {
        auto xSize = 1 + max_element( dots.begin(), dots.end(), [](auto d1, auto d2){ return d1.first < d2.first ; } )->first;
        auto ySize = 1 + max_element( dots.begin(), dots.end(), [](auto d1, auto d2){ return d1.second < d2.second ; } )->second;
        string blank( xSize, ' ' );
        vector<string> display( ySize );
        fill( display.begin(), display.end(), blank );
        for ( auto dot: dots )
        {
            auto x = dot.first;
            auto y = dot.second;
            display[y][x] = '#';
        }
        cout << endl;
        for ( auto line: display )
        {
            cout << line << endl;
        }
        cout << endl;
    }

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
