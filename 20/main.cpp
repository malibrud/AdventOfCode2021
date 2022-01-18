#include <vector>
#include <regex>
#include <algorithm>
#include <set>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;

vector<Test> partATests =
{
    {"test1.txt", "2", "35"},
};
string paramA = "2"; 
 
vector<Test> partBTests =
{
    {"test1.txt", "", ""},
};
string paramB = "";

void printImage( vector< vector<int> >& image );

string computePartA( string fileName, string param )
{
    // Convert the number of iterations
    int nIter = stoi( param );

    // Read the data
    auto lines = readLines( fileName );

    // Compute parameters
    int nLines  = lines.size();
    int nRows   = nLines - 2;
    int nCols   = lines[2].size();
    int nBits   = lines[0].size();
    int padding = 3 + nIter;
    int W       = nRows + 2 * padding;
    int H       = nCols + 2 * padding;

    // Read the algorithm
    vector<int> algorithm( nBits, 0 );
    for ( int i = 0 ; i < nBits ; i++ ) 
    {
        algorithm[i] = lines[0][i] == '#' ? 1 : 0;
    }

    // Read the image
    vector< vector<int> > image( H, vector<int>( W, 0 ) );
    for (int i = 0 ; i < nRows ; i++ )
    {
        int r = i + padding;
        auto& line = lines[i + 2];
        for ( int j = 0 ; j < nCols ; j++ )
        {
            int c = j + padding;
            image[r][c] = line[j] == '#' ? 1 : 0;
        }
    }

    // Process the image
    auto dest = image;
    // printImage( image );
    for ( int iter = 0 ; iter < nIter ; iter++ )
    {
        for ( int i = 1 ; i < H-1 ; i++ )
        {
            for ( int j = 1 ; j < W-1 ; j++ )
            {
                int index = 0;
                int bit   = 8;
                for ( int r = -1 ; r <= 1 ; r++)
                    for ( int c = -1 ; c <= 1 ; c++)
                        index += image[i+r][j+c] << bit--;
                dest[i][j] = algorithm[index];
            }
        }
        image = dest;

        // Fill out the borders to represent the continuation to infinity.
        auto infVal = image[1][1]; 
        for ( int i = 0 ; i < H ; i++ )
        {
            image[i][0] = image[i][H-1] = infVal;
        }
        for ( int i = 0 ; i < W ; i++ )
        {
            image[0][i] = image[W-1][i] = infVal;
        }
        
        // cout << "\nIteration " << iter+1 << endl;
        // printImage( image );
    }

    // Count the number of lit pixels
    int count = 0;
    for ( int i = 1 ; i < H-1 ; i++ )
        for ( int j = 1 ; j < W-1 ; j++ )
            count += image[i][j];

    return to_string( count );
}

void printImage( vector< vector<int> >& image )
{
    for ( auto& row: image )
    {
        for ( auto& c: row )
        {
            char val = c == 1 ? '#' : '.';
            cout << val;
        }
        cout << endl;
    }
}

string computePartB( string fileName, string param )
{
    return to_string( 0 );
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
