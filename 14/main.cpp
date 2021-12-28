#include <map>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;

vector<Test> partATests =
{
    {"test1.txt", "10", "1588"}
};
string paramA = "10"; 
 
vector<Test> partBTests =
{
    {"test1.txt", "40", "2188189693529"}
};
string paramB = "40";

string computePartA( string fileName, string param )
{
    auto nSteps = stoi( param );
    auto lines = readLines( fileName );
    int nLines = lines.size();
    auto poly = lines[0];
    map<string, char> rules;
    for ( int i = 2 ; i < nLines ; i++)
    {
        auto &line = lines[i];
        auto key = line.substr(0,2);
        auto inschar = line[6];
        rules[key] = inschar;
    }

    for ( auto step = 0 ; step < nSteps ; step++ )
    {
        auto len = poly.size();
        string newPoly;
        newPoly.reserve( 2 * len );

        for ( int i = 0 ; i < len - 1 ; i++)
        {
            auto key = poly.substr( i, 2 );
            if ( rules.find( key ) != rules.end() )
            {
                newPoly = newPoly + poly[i] + rules[key];
            }
            else
            {
                newPoly += poly[i];
            }
        }
        newPoly += poly[ len-1 ];
        poly = newPoly;
    }

    int counts[26] = {0};
    for (auto c: poly)
    {
        counts[ c - 'A' ]++;
    }
    auto minCount = (int)poly.size();
    auto maxCount = 0;
    for ( int i = 0 ; i < 26 ; i++)
    {
        if (counts[i] == 0) continue;
        minCount = min( minCount, counts[i] );
        maxCount = max( maxCount, counts[i] );
    }

    auto diff = maxCount - minCount;

    return to_string(diff);
}

typedef long long int64;
struct PairCount
{
    int64 nPairs;
    int64 nFirst;
    int64 nSecond;

    PairCount()
        : nPairs(0)
        , nFirst(0)
        , nSecond(0)
    {}

    PairCount( int64 n, int64 n1, int64 n2 )
        : nPairs(n)
        , nFirst(n1)
        , nSecond(n2)
    {}
};

void addCountsToKey( map<string, PairCount> &pairs, string &key, PairCount &counts );

string computePartB( string fileName, string param )
{
    auto nSteps = stoi( param );
    auto lines = readLines( fileName );
    int nLines = lines.size();
    auto poly = lines[0];
    map<string, char> rules;
    for ( int i = 2 ; i < nLines ; i++)
    {
        auto &line = lines[i];
        auto key = line.substr(0,2);
        auto inschar = line[6];
        rules[key] = inschar;
    }

    map< string, PairCount > polyCounts;

    string key;
    for (int i = 0 ; i < poly.size() - 1 ; i++)
    {
        key = poly.substr( i, 2 );
        addCountsToKey( polyCounts, key, PairCount( 1, 1, 0 ) );
    }
    polyCounts[ key ].nSecond = 1;

    for ( int step = 0 ; step < nSteps ; step++ )
    {
        map<string, PairCount> newPolyCounts;
        for ( auto &p: polyCounts)
        {
            auto &key = p.first;
            auto &counts = p.second;

            if ( counts.nPairs == 0 ) continue;

            auto insChar = rules[ key ];

            auto k1 = string( { key[0], insChar } );
            auto k2 = string( { insChar, key[1] } );

            addCountsToKey( newPolyCounts, k1, PairCount(counts.nPairs, counts.nFirst, counts.nPairs) );
            addCountsToKey( newPolyCounts, k2, PairCount(counts.nPairs,             0, counts.nSecond) );
        }
        polyCounts = newPolyCounts;
    }

    int64 counts[26] = {0};
    for (auto &pc: polyCounts)
    {   
        auto &k = pc.first;
        auto &c = pc.second;
        
        counts[ k[0] - 'A' ] += c.nFirst;
        counts[ k[1] - 'A' ] += c.nSecond;
    }
    int64 minCount = numeric_limits<long long int>::max();
    int64 maxCount = 0;
    for ( int i = 0 ; i < 26 ; i++)
    {
        if (counts[i] == 0) continue;
        minCount = min( minCount, counts[i] );
        maxCount = max( maxCount, counts[i] );
    }

    auto diff = maxCount - minCount;

    return to_string(diff);
}

void addCountsToKey( map<string, PairCount> &pairs, string &key, PairCount &counts )
{
    if ( pairs.count( key ) == 0 )
    {
        // Key does not exist in the map
        pairs[ key ] = counts;
    }
    else
    {
        // Key does exist in the map
        pairs[ key ].nPairs  += counts.nPairs;
        pairs[ key ].nFirst  += counts.nFirst;
        pairs[ key ].nSecond += counts.nSecond;

        auto &check = pairs[ key ];
        if ( check.nPairs < 0 || check.nFirst < 0 || check.nSecond < 0 )
        {
            throw "Int64 overflow";
        }
    }
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
