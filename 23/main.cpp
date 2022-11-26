#include <regex>
#include <set>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;

vector<Test> partATests =
{
    {"test1.txt", "", "12521"},
};
string paramA = ""; 
 
vector<Test> partBTests =
{
    {"test1.txt", "", ""},
};
string paramB = "";

struct Node
{
    int energy; // Total Energy to get to this state.

    char hall[11] = {'.'};
    char roomA[2];
    char roomB[2];
    char roomC[2];
    char roomD[2];
    
    int f;  // A* total cost
    int g;  // A* cost
    int h;  // A* heuristic cost

    bool inOpenSet;

    Node *prev;
};

struct Compare
{
    bool operator()(Node *n1, Node *n2)
    {
        return n1->f > n2->f;
    }
};

string computePartA( string fileName, string param )
{
    // Read the data
    auto lines = readLines( fileName );

    Node start;
    start.roomA[0] = lines[2][3];
    start.roomA[1] = lines[3][3];
    start.roomB[0] = lines[2][5];
    start.roomB[1] = lines[3][5];
    start.roomC[0] = lines[2][7];
    start.roomC[1] = lines[3][7];
    start.roomD[0] = lines[2][9];
    start.roomD[1] = lines[3][9];
    
    return to_string( 0 );
}

string computePartB( string fileName, string param )
{
    // Read the data
    auto lines = readLines( fileName );

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
