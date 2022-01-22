#include <regex>
#include <set>

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
    {"test3.txt", "", "2758514936282235"},
};
string paramB = "";

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

int getElementIndex(vector<int>& vec, int e)
{
    for (int i = 0 ; i < vec.size() ; i++ )
    {
        if (vec[i] == e) return i;
    }
    return -1;
}


struct Cuboid
{
    bool isOn = false;;
    int x1, x2;
    int y1, y2;
    int z1, z2;
};

string computePartB( string fileName, string param )
{
    // Read the data
    auto lines = readLines( fileName );

    regex cuboidPattern( R"((on|off) x=(-?\d+)..(-?\d+),y=(-?\d+)..(-?\d+),z=(-?\d+)..(-?\d+))" );
    smatch match;

    vector<Cuboid> cuboids;

    // the x, y, and z arrays represent the list of ordered boundary points between the cuboids.
    vector<int> x;
    vector<int> y;
    vector<int> z;

    for ( auto& line : lines )
    {
        regex_match( line, match, cuboidPattern );
        Cuboid cu;
        cu.isOn = match[1] == "on";
        cu.x1   = stoi( match[2] );
        cu.x2   = stoi( match[3] );
        cu.y1   = stoi( match[4] );
        cu.y2   = stoi( match[5] );
        cu.z1   = stoi( match[6] );
        cu.z2   = stoi( match[7] );

        x.push_back(cu.x1);
        x.push_back(cu.x2+1);
        y.push_back(cu.y1);
        y.push_back(cu.y2+1);
        z.push_back(cu.z1);
        z.push_back(cu.z2+1);

        cuboids.push_back(cu);
    }

    // Create an ordered list of possible borders between each cell.
    // Each border is inclusive at the beginning and exclusive at the end.
    sort(x.begin(), x.end());
    sort(y.begin(), y.end());
    sort(z.begin(), z.end());

    auto newXend = unique(x.begin(), x.end());
    auto newYend = unique(y.begin(), y.end());
    auto newZend = unique(z.begin(), z.end());
    
    x.resize( distance( x.begin(), newXend) );
    y.resize( distance( y.begin(), newYend) );
    z.resize( distance( z.begin(), newZend) );
    int nx = x.size();
    int ny = y.size();
    int nz = z.size();

    // This array holds information as to whether a whole cell is
    // on or off.
    // Allocate an array to hold the cell values.
    bool ***cells;
    cells = (bool***)malloc(nx*sizeof(bool**));
    for ( int i = 0 ; i < nx ; i++ ) 
    {
        cells[i] = (bool**)malloc(ny*sizeof(bool*));
        for (int j = 0 ; j < ny ; j++ )
        {
            cells[i][j] = (bool*)malloc(nz*sizeof(bool));
            for (int k = 0 ; k < nz ; k++ )
            {
                cells[i][j][k] = false;
            }
        }
    }

    // Cycle through all cuboids and find respective cells and flip them accordingly.
    for (auto& cu : cuboids )
    {
        int ix1 = getElementIndex( x, cu.x1 );
        int ix2 = getElementIndex( x, cu.x2+1 );
        int iy1 = getElementIndex( y, cu.y1 );
        int iy2 = getElementIndex( y, cu.y2+1 );
        int iz1 = getElementIndex( z, cu.z1 );
        int iz2 = getElementIndex( z, cu.z2+1 );

        for ( int i = ix1 ; i < ix2 ; i++ )
        for ( int j = iy1 ; j < iy2 ; j++ )
        for ( int k = iz1 ; k < iz2 ; k++ )
        {
            cells[i][j][k] = cu.isOn;
        }
    }

    // Find each cell that is on and add its volume to the count.
    int64_t count = 0;
    for ( int i = 0 ; i < nx ; i++ )
    for ( int j = 0 ; j < ny ; j++ )
    for ( int k = 0 ; k < nz ; k++ )
    {
        bool isOn = cells[i][j][k];
        if ( !isOn ) continue;

        int64_t dx = x[i+1] - x[i];
        int64_t dy = y[j+1] - y[j];
        int64_t dz = z[k+1] - z[k];
        count += dx * dy * dz;
    }

    // Being a good citizen and cleaning up allocated memory.
    for ( int i = 0 ; i < nx ; i++ ) 
    {
        for (int j = 0 ; j < ny ; j++ )
        {
            free( cells[i][j] );
        }
        free( cells[i] );
    }
    free( cells ); 

    return to_string( count );
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
