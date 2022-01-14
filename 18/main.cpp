#include <cassert> 
#include <regex>
#include <cmath>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;

vector<Test> partATests =
{
    {"test1.txt", "", "4140"},
};
string paramA = ""; 
 
vector<Test> partBTests =
{
    {"test1.txt", "", "3993"},
};
string paramB = "";

string add( string &a, string &b);
void reduce( string &sum );
bool explode (string &num );
void explodePairAtIndex( string &num, int idx );
void explodeLeft( string &str, int val );
void explodeRight( string &str, int val );
bool split( string &num );
int getMagnitude( string &num, int idx, int &mag );

string computePartA( string fileName, string param )
{
    auto lines = readLines( fileName );

    string sum = lines[ 0 ];
    reduce( sum );

    for ( int i = 1 ; i < lines.size() ; i++ )
    {
        sum = add( sum, lines[i] );
        reduce( sum );
    }

    int mag;
    getMagnitude( sum, 0, mag );   

    return to_string( mag );
}

string computePartB( string fileName, string param )
{
    auto lines = readLines( fileName );
    auto N = lines.size();

    int maxMag = 0;
    for ( int i = 0 ; i < N ; i++ )
    {
        for ( int j = i + 1 ; j < N ; j++ )
        {
            string sum;
            int mag;

            auto &n1 = lines[ i ];
            auto &n2 = lines[ j ];

            sum = add( n1, n2 );
            reduce( sum );
            getMagnitude( sum, 0, mag );
            maxMag = max( maxMag, mag );
            
            sum = add( n2, n1 );
            reduce( sum );
            getMagnitude( sum, 0, mag );
            maxMag = max( maxMag, mag );
        }
    }
    return to_string( maxMag );
}

string add( string &a, string &b)
{
    return string("[") + a + "," + b + "]";
}

void reduce( string &num )
{
    bool actionTaken = true;

    while ( actionTaken )
    {
        actionTaken = explode( num );
        if ( actionTaken ) continue;
        actionTaken = split( num );
    }
}

bool explode (string &num )
{
    int depth = 0;
    for ( int i = 0 ; i < num.size() ; i++ )
    {
        char c = num[i];
        if ( c == '[' ) 
        {
            depth++;
            if ( depth == 5 )
            {
                explodePairAtIndex( num, i );
                return true;
            }
        }
        else if ( c == ']' ) 
        {
            depth--;
        }
    }
    return false;
}

void explodePairAtIndex( string &num, int idx )
{
    regex pattern( R"(\[(\d+),(\d+)\])" );
    smatch match;
    auto partNum = num.substr(idx);
    if ( regex_search( partNum, match, pattern ) )
    {
        auto lval = stoi( match[1] );
        auto lpart = num.substr( 0, idx );
        explodeLeft( lpart, lval );

        auto rval = stoi( match[2] );
        auto rpart = match.suffix().str();
        explodeRight( rpart, rval );

        num = lpart + "0" + rpart;
    }
}

void explodeLeft( string &str, int val )
{
    regex pattern( R"((\d+)[^\d]*$)");
    smatch match;
    if ( regex_search( str, match, pattern ) && !match.empty() )
    {
        int oldVal = stoi( match[1] );
        int newVal = oldVal + val;
        auto prefix = match.prefix().str();
        auto len = match[1].str().size();
        auto suffix = match[0].str().substr( len );
        str = prefix + to_string(newVal) + suffix;
    }
}

void explodeRight( string &str, int val )
{
    regex pattern( R"(\d+)");
    smatch match;
    if ( regex_search( str, match, pattern ) )
    {
        int oldVal = stoi( match[0] );
        int newVal = oldVal + val;
        auto prefix = match.prefix().str();
        auto suffix = match.suffix().str();
        str = prefix + to_string(newVal) + suffix;
    }
}

bool split( string &num )
{
    regex pattern( R"(\d{2,})");
    smatch match;
    if ( regex_search( num, match, pattern) )
    {
        string prefix = match.prefix().str();
        string suffix = match.suffix().str();
        int val = stoi( match[0] );
        int lval = val/2;
        int rval = val - lval;
        string newPair = "[" + to_string(lval) + "," + to_string(rval) + "]";
        num = prefix + newPair + suffix;
        return true;
    }
    return false;
}

int getMagnitude( string &num, int idx, int &mag )
{
    int lmag; 
    int rmag;
    // Get the magnitude of a number
    if ( num[ idx ] >= '0' && num[ idx ] <= '9')
    {
        mag = stoi( num.substr( idx++, 1 ) );
    }
    // Get the magnitude of a pair.
    else if ( num[ idx ] == '[' )
    {
        idx++;
        idx = getMagnitude( num, idx, lmag);
        if ( num[ idx++ ] != ',' ) 
            cout << "Error: Expecting a ','\n";
        idx = getMagnitude( num, idx, rmag);
        if ( num[ idx++ ] != ']' ) 
            cout << "Error: Expecting a ']'\n";
        mag = 3 * lmag + 2 * rmag;
    }
    return idx;
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
