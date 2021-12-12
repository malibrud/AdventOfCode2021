#include <algorithm>
#include <numeric>
#include <cmath>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;


vector<Test> partATests =
{
    {"test.txt", "", "37"}
};
string paramA = ""; 
 
vector<Test> partBTests =
{
    {"test.txt", "", "168"} 
};
string paramB = "";

string computePartA( string fileName, string param )
{
    auto lines = readLines(fileName);
    auto crabs = parseCsvInts(lines[0]);
    auto N = crabs.size();
    auto sum = 0;
    auto minPos = *min_element(crabs.begin(), crabs.end());
    auto maxPos = *max_element(crabs.begin(), crabs.end());

    auto minFuel = INT32_MAX;
    for (auto pos = minPos ; pos <= maxPos ; pos++)
    {
        auto fuel = 0;
        for (auto crab: crabs) fuel += abs(crab - pos);
        minFuel = min(minFuel, fuel);
    }

    return to_string(minFuel);
}

string computePartB( string fileName, string param )
{
    auto lines = readLines(fileName);
    auto crabs = parseCsvInts(lines[0]);
    auto N = crabs.size();
    auto sum = 0;
    auto minPos = *min_element(crabs.begin(), crabs.end());
    auto maxPos = *max_element(crabs.begin(), crabs.end());

    auto minFuel = INT32_MAX;
    for (auto pos = minPos ; pos <= maxPos ; pos++)
    {
        auto totalFuel = 0;
        for (auto crab: crabs) 
        {
            auto dist = abs(crab - pos);
            auto fuel = dist * (dist + 1) / 2;
            totalFuel += fuel;
        }
        minFuel = min(minFuel, totalFuel);
    }

    return to_string(minFuel);
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
