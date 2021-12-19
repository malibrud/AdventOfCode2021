#include <iostream>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;


vector<Test> partATests =
{
    {"test.txt", "100", "1656"}
};
string paramA = "100"; 
 
vector<Test> partBTests =
{
    {"test.txt", "", "195"} 
};
string paramB = "";

const int N = 10;
void flashCell(int grid[N][N], int r, int c);

string computePartA( string fileName, string param )
{
    auto lines = readLines(fileName);
    auto S = stoi(param);

    int grid[N][N];

    // Populate the grid.
    for (int r = 0 ; r < N ; r++)
        for (int c = 0 ; c < N ; c++)
            grid[r][c] = lines[r][c] - '0';

    int totalFlashes = 0;
    for (int step = 1 ; step <= S ; step++)
    {
        // Update the grid.
        for (int r = 0 ; r < N ; r++)
            for (int c = 0 ; c < N ; c++)
                grid[r][c]++;

        // Simulate flashes
        for (int r = 0 ; r < N ; r++)
            for (int c = 0 ; c < N ; c++)
                if ( grid[r][c] == 10 )
                    flashCell(grid, r, c);

        // count flashes
        int flashes = 0;
        for (int r = 0 ; r < N ; r++)
            for (int c = 0 ; c < N ; c++)
                if ( grid[r][c] == 0 )
                    flashes++;
        totalFlashes += flashes;
        
        // Output intermediate results
        /*
        cout << "Step " << step << " (" << flashes << ", " << totalFlashes << ")" << endl;
        for (int r = 0 ; r < N ; r++)
        {
            for (int c = 0 ; c < N ; c++)
            {
                auto g = grid[r][c];
                if (g == 0) cout << '.';
                else cout << grid[r][c];
            }
            cout << endl;
        }
        cout << endl;
        */

    }
    return to_string(totalFlashes);
}


string computePartB( string fileName, string param )
{
    auto lines = readLines(fileName);

    int grid[N][N];

    // Populate the grid.
    for (int r = 0 ; r < N ; r++)
        for (int c = 0 ; c < N ; c++)
            grid[r][c] = lines[r][c] - '0';

    int step = 1;
    while (true)
    {
        // Update the grid.
        for (int r = 0 ; r < N ; r++)
            for (int c = 0 ; c < N ; c++)
                grid[r][c]++;

        // Simulate flashes
        for (int r = 0 ; r < N ; r++)
            for (int c = 0 ; c < N ; c++)
                if ( grid[r][c] == 10 )
                    flashCell(grid, r, c);

        // Check if all flashed.
        auto allFlashed = true;
        for (int r = 0 ; r < N ; r++)
            for (int c = 0 ; c < N ; c++)
                allFlashed = allFlashed && ( grid[r][c] == 0 );
         
        if (allFlashed) break;

        step++;
    }
    return to_string(step);
}

const int adjacent[][2]
{
    // top row
    {-1, -1},
    {-1,  0},
    {-1,  1},

    // middle row
    { 0, -1},
    { 0,  1},

    // bottom row
    { 1, -1},
    { 1,  0},
    { 1,  1}
};

void flashCell(int grid[N][N], int r, int c)
{
    grid[r][c] = 0;  // mark cell as flashed.

    // Walk around to the 8 adjacent cells.
    for (int i = 0 ; i < 8 ; i++)
    {
        int ir = r + adjacent[i][0];
        int ic = c + adjacent[i][1];

        // Did we fall off of the grid?
        if (ir < 0 || ir >= N) continue;
        if (ic < 0 || ic >= N) continue;

        if (grid[ir][ic] == 0) continue;
        if (grid[ir][ic] == 10)
        {
            flashCell(grid, ir, ic);
            continue;
        }
        grid[ir][ic]++;
        if (grid[ir][ic] == 10) 
        {
            flashCell(grid, ir, ic);
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
