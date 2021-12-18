#include <algorithm>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;


vector<Test> partATests =
{
    {"test.txt", "", "15"}
};
string paramA = ""; 
 
vector<Test> partBTests =
{
    {"test.txt", "", "1134"} 
};
string paramB = "";

string computePartA( string fileName, string param )
{
    auto lines = readLines(fileName);
    auto R = lines.size();
    auto C = lines[0].size();

    // Cycle through the points to find minimum values.
    int totalRisk = 0;
    for (int i = 0 ; i < R ; i++) 
    {
        for (int j = 0 ; j < C ; j++) 
        {
            auto val = lines[i][j];
            auto up    = i > 0   ? lines[i-1][j] : '9';
            auto down  = i < R-1 ? lines[i+1][j] : '9';
            auto left  = j > 0   ? lines[i][j-1] : '9';
            auto right = j < C-1 ? lines[i][j+1] : '9';
            if (val < up && val < down && val < left && val < right)
            {
                auto risk = val - '0' + 1;
                totalRisk += risk;
            }
        }
    }

    return to_string(totalRisk);
}

int markUphillPoints(vector<string> &map, int r, int c);

string computePartB( string fileName, string param )
{
    auto map = readLines(fileName);
    auto R = map.size();
    auto C = map[0].size();

    vector<int> basinSizes;
    // Cycle through the points to find minimum values.
    for (int i = 0 ; i < R ; i++) 
    {
        for (int j = 0 ; j < C ; j++) 
        {
            auto val = map[i][j];
            auto up    = i > 0   ? map[i-1][j] : '9';
            auto down  = i < R-1 ? map[i+1][j] : '9';
            auto left  = j > 0   ? map[i][j-1] : '9';
            auto right = j < C-1 ? map[i][j+1] : '9';
            if (val < up && val < down && val < left && val < right)
            {
                basinSizes.push_back( markUphillPoints( map, i, j) );
            }
        }
    }
    sort(basinSizes.begin(), basinSizes.end(), [](int i, int j){ return i > j; } );
    auto product = basinSizes[0] * basinSizes[1] * basinSizes[2]; 

    // for (auto r : map) cout << r << endl;

    return to_string(product);
}

int markUphillPoints(vector<string> &map, int r, int c)
{
    if ( map[r][c] == '*' ) return 0;

    auto R   = map.size();
    auto C   = map[0].size();
    auto val = map[r][c];

    auto sum = 0;
    if ( r > 0 && val < map[r-1][c] && map[r-1][c] < '9')
    {
        sum += markUphillPoints(map, r-1, c);
    }
    if ( r < R-1 && val < map[r+1][c] && map[r+1][c] < '9')
    {
        sum += markUphillPoints(map, r+1, c);
    }
    if ( c > 0 && val < map[r][c-1] && map[r][c-1] < '9')
    {
        sum += markUphillPoints(map, r, c-1);
    }
    if ( c < C-1 && val < map[r][c+1] && map[r][c+1] < '9')
    {
        sum += markUphillPoints(map, r, c+1);
    }
    map[r][c] = '*';
    return sum + 1;
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
