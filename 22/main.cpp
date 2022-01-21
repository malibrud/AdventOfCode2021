#include <regex>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;

vector<Test> partATests =
{
    {"test1.txt", "", "39"},
    {"test2.txt", "", "590784"},
};
string paramA = ""; 
 
vector<Test> partBTests =
{
    {"test1.txt", "", ""},
};
string paramB = "";

struct Cuboid
{
    bool on;
    int x1, x2;
    int y1, y2;
    int z1, z2;
};

bool computer[101][101][101] = {false};
void resetComputer()
{
    for ( int x = -50 ; x <= +50 ; x++ )
    for ( int y = -50 ; y <= +50 ; y++ )
    for ( int z = -50 ; z <= +50 ; z++ )
    {
        computer[x+50][y+50][z+50] = false;
    }
}

string computePartA( string fileName, string param )
{
    // Read the data
    auto lines = readLines( fileName );

    regex cuboidPattern( R"((on|off) x=(-?\d+)..(-?\d+),y=(-?\d+)..(-?\d+),z=(-?\d+)..(-?\d+))" );
    smatch match;
    // vector<Cuboid> cuboids;

    // Reset the computer from prior runs
    resetComputer();

    int count = 0;
    for ( auto& line : lines )
    {
        regex_match( line, match, cuboidPattern );
        bool isOn = match[1] == "on";
        int x1    = stoi( match[2] );
        int x2    = stoi( match[3] );
        int y1    = stoi( match[4] );
        int y2    = stoi( match[5] );
        int z1    = stoi( match[6] );
        int z2    = stoi( match[7] );

        if (
            x2 < -50 || x1 > 50 ||
            y2 < -50 || y1 > 50 ||
            z2 < -50 || z1 > 50
        ) continue;
        
        count++;

        for ( int x = x1 ; x <= x2 ; x++ )
        for ( int y = y1 ; y <= y2 ; y++ )
        for ( int z = z1 ; z <= z2 ; z++ )
        {
            computer[x+50][y+50][z+50] = isOn;
        }
    }

    count = 0;
    for ( int x = -50 ; x <= +50 ; x++ )
    for ( int y = -50 ; y <= +50 ; y++ )
    for ( int z = -50 ; z <= +50 ; z++ )
    {
        if ( computer[x+50][y+50][z+50] ) count++;
    }

    return to_string( count );
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
