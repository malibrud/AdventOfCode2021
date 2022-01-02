#include <regex>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;

vector<Test> partATests =
{
    {"test1.txt", "", "45"},
};
string paramA = ""; 
 
vector<Test> partBTests =
{
    {"test1B.txt", "", "0"},
};
string paramB = "";

string computePartA( string fileName, string param )
{
    auto lines = readLines( fileName );
    auto targetArea = lines[0];

    regex pattern( R"(target area: x=([-+]?\d+)\.\.([-+]?\d+), y=([-+]?\d+)..([-+]?\d+))" );
    smatch match;
    regex_match( targetArea, match, pattern );
    int xmin = stoi( match[1] );
    int xmax = stoi( match[2] );
    int ymin = stoi( match[3] );
    int ymax = stoi( match[4] );

    // Determine min and max x velocity.
    // Formulas below were derived from the sum 1...N = N(N+1)/2 solving for N given the sum.
    int vxmin = (int)floor( ( -1 + sqrt( 1 + 8 * xmin ) ) / 2 );
    int vxmax = (int)ceil( ( -1 + sqrt( 1 + 8 * xmax ) ) / 2 );

    // Determine min and max y velocity.
    // Because of the symmetry of the problem the probe will
    // always return to y-0 with -v velocity.  So if the probe 
    // is at 0 with a velocity > ymin then the probe will always skip past
    // the target area.
    int vymin = 0;
    int vymax = -ymin;

    // now simulate all possible trajectories
    int hmaxmax = 0;
    for ( auto vxi = vxmin ; vxi <= vxmax ; vxi++)
    {
        for ( auto vyi = vymin ; vyi <= vymax ; vyi++)
        {
            //cout << "Trying: " << vxi << " " << vyi << ": ";
            int x = 0;
            int y = 0;
            int vx = vxi;
            int vy = vyi;
            int hmax = 0;
            while ( y >= ymin )
            {
                x += vx;
                y += vy;
                if (vx > 0) vx--;
                vy--;
                hmax = max( hmax, y );

                // Check if we hit the target area
                if ( x >= xmin && x <= xmax && y >= ymin && y <= ymax )
                {
                    //cout << "Hit " << hmax << endl;
                    hmaxmax = max( hmaxmax, hmax );
                    break;
                }
            }
            //cout << endl;
        }

    } 
    return to_string( hmaxmax );
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
