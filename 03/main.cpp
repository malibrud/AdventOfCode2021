
#include "../common/testing.h"
#include "../common/readers.h"

using namespace std;

vector<Test> partATests =
{
    {"test.txt", "", "198"} 
};
 
vector<Test> partBTests =
{
    {"test.txt", "", "230"} 
};

string computePartA( string fileName, string param )
{
    auto lines = readLines( fileName );
    int nLines = lines.size();
    int nBits  = lines[0].length();

    int gamma   = 0;
    int epsilon = 0;
    for (int bit = 0 ; bit < nBits ; bit++)
    {
        int idx = (nBits - 1) - bit;
        int count = 0;
        for (auto line : lines)
        {
            if (line[idx] == '1') count ++;
        }
        if (count > nLines/2) 
            gamma = gamma | 1 << bit;
        else 
            epsilon = epsilon | 1 << bit;
    }
    
    auto power = gamma * epsilon;
    return to_string(power);
}

string computePartB( string fileName, string param )
{
    auto lines = readLinesAsList( fileName );
    int nBits  = lines.front().length();

    // Get O2 Generator Rating
    auto O2List(lines);
    for (int bit = 0 ; bit < nBits ; bit++)
    {
        // Are we done?
        auto N = O2List.size();
        if (N == 1) break;

        // Count the 1 bits
        int ones = 0;
        for (auto line : O2List)
        {
            if (line[bit] == '1') ones ++;
        }
        
        // Which character to keep
        auto zeros = N - ones;
        auto keep = ones >= zeros ? '1' : '0';

        O2List.remove_if( [bit, keep](auto x){ return x[bit] != keep ; } );
    }
    auto O2Rating = stoi(O2List.front(), 0, 2);

    // Get CO2 Scrubber Rating
    auto CO2List(lines);
    for (int bit = 0 ; bit < nBits ; bit++)
    {
        // Are we done?
        auto N = CO2List.size();
        if (N == 1) break;

        // Count the 1 bits
        int ones = 0;
        for (auto line : CO2List)
        {
            if (line[bit] == '1') ones ++;
        }
        
        // Which character to keep
        auto zeros = N - ones;
        auto keep = zeros <= ones ? '0' : '1';

        CO2List.remove_if( [bit, keep](auto x){ return x[bit] != keep ; } );
    }
    auto CO2Rating = stoi(CO2List.front(), 0, 2);
    
    auto rating = O2Rating * CO2Rating;

    return to_string(rating);
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        runPart("A", computePartA, partATests);
        runPart("B", computePartB, partBTests);
    }
    else if (string(argv[1]) == "A")
    {
        runPart("A", computePartA, partATests);
    }
    else if (string(argv[1]) == "B")
    {
        runPart("B", computePartB, partBTests);
    }
}
