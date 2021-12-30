#include <vector>
#include <limits>
#include <queue>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;

vector<Test> partATests =
{
    {"test1.txt", "", "40"}
};
string paramA = ""; 
 
vector<Test> partBTests =
{
    {"test1.txt", "", "315"}
};
string paramB = "";

struct Node
{
    int risk;
    int r, c;   // Row and column of the node.
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
    auto lines = readLines( fileName );
    int C = lines[0].size();
    int R = lines.size();

    vector< vector<Node> > grid;
    for (auto r = 0 ; r < R ; r++)
    {
        string &line = lines[r];
        vector<Node> row;
        for (auto c = 0 ; c < C ; c++)
        {
            Node node;
            node.risk = line[c] - '0';
            node.r = r;
            node.c = c;
            node.f = INT32_MAX;
            node.g = INT32_MAX;
            node.h = INT32_MAX;
            node.inOpenSet = false;
            row.push_back( node );
        }
        grid.push_back( row );
    }

    // Initialize the start node at 0, 0.
    grid[0][0].g = 0;
    grid[0][0].h = R - 1 + C - 1;
    grid[0][0].f = grid[0][0].g  + grid[0][0].h;

    // Push it onto the open set.
    priority_queue<Node*, vector<Node*>, Compare> openSet;
    openSet.push(&grid[0][0]);
    grid[0][0].inOpenSet = true;

    int directions[4][2] = 
    {
        {-1,  0}, 
        { 1,  0}, 
        { 0, -1}, 
        { 0,  1}
    };

    while ( !openSet.empty() )
    {
        auto current = openSet.top();
        openSet.pop();
        current->inOpenSet = false;

        if ( current->r == R-1 && current->c == C-1 ) break;

        for ( auto d = 0 ; d < 4 ; d++)
        {
            auto c = current->c + directions[d][0];
            auto r = current->r + directions[d][1];
            if ( r < 0 || r >= R ) continue;
            if ( c < 0 || c >= C ) continue;

            Node &neighbor = grid[r][c];
            auto g = current->g + neighbor.risk;
            if ( g < neighbor.g )
            {
                neighbor.g = g;
                neighbor.h = R-r-1 + C-c-1;
                neighbor.f = neighbor.g + neighbor.h;
                neighbor.prev = current;
                if ( !neighbor.inOpenSet )
                {
                    openSet.push( &neighbor );
                    neighbor.inOpenSet = true;
                }
            }
        }
    }
    return to_string( grid[R-1][C-1].g );
}

string computePartB( string fileName, string param )
{
    auto lines = readLines( fileName );
    int R = lines.size();       // Rows in one tile
    int C = lines[0].size();    // Cols in one tile
    int RR = 5*R;               // Total rows in all tiles
    int CC = 5*C;               // Total cols in all tiles

    vector< vector<Node> > grid;
    grid.reserve( RR );
    for (auto r = 0 ; r < RR ; r++)
    {
        auto rt = r / R;    // Row tile
        auto ri = r % R;    // Row index within the tile

        string &line = lines[ri];
        vector<Node> row;
        row.reserve( CC );
        for (auto c = 0 ; c < CC ; c++)
        {
            auto ct = c / C;    // Column tile
            auto ci = c % C;    // Column index within the tile
            
            // Implement risk rule for the additional tiles.
            int risk = line[ci] - '0';
            risk += rt + ct;
            if ( risk > 9 ) risk -= 9;

            Node node;
            node.risk = risk;
            node.r = r;
            node.c = c;
            node.f = INT32_MAX;
            node.g = INT32_MAX;
            node.h = INT32_MAX;
            node.inOpenSet = false;
            row.push_back( node );
        }
        grid.push_back( row );
    }


    // Initialize the start node at 0, 0.
    grid[0][0].g = 0;
    grid[0][0].h = RR - 1 + CC - 1;
    grid[0][0].f = grid[0][0].g  + grid[0][0].h;

    // Push it onto the open set.
    priority_queue<Node*, vector<Node*>, Compare> openSet;
    openSet.push(&grid[0][0]);
    grid[0][0].inOpenSet = true;

    int directions[4][2] = 
    {
        {-1,  0}, 
        { 1,  0}, 
        { 0, -1}, 
        { 0,  1}
    };

    while ( !openSet.empty() )
    {
        auto current = openSet.top();
        openSet.pop();
        current->inOpenSet = false;

        if ( current->r == RR-1 && current->c == CC-1 ) break;

        for ( auto d = 0 ; d < 4 ; d++)
        {
            auto c = current->c + directions[d][0];
            auto r = current->r + directions[d][1];
            if ( r < 0 || r >= RR ) continue;
            if ( c < 0 || c >= CC ) continue;

            Node &neighbor = grid[r][c];
            auto g = current->g + neighbor.risk;
            if ( g < neighbor.g )
            {
                neighbor.g = g;
                neighbor.h = RR-r-1 + CC-c-1;
                neighbor.f = neighbor.g + neighbor.h;
                neighbor.prev = current;
                if ( !neighbor.inOpenSet )
                {
                    openSet.push( &neighbor );
                    neighbor.inOpenSet = true;
                }
            }
        }
    }
    return to_string( grid[RR-1][CC-1].g );
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
