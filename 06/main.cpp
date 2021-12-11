#include <regex>
#include <cmath>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;


vector<Test> partATests =
{
    {"test.txt", "18", "26"},
    {"test.txt", "80", "5934"} 
};
string paramA = "80"; // days
 
vector<Test> partBTests =
{
    {"test.txt", "256", "26984457539"} 
};
string paramB = "256";

typedef unsigned long long uint64;
string computePartA( string fileName, string param )
{
    auto days = stoi(param);
    auto lines = readLines(fileName);
    auto allFish = parseCsvInts(lines[0]);
    uint64 numberPerDay[9] = {0};
    for (auto fish : allFish)
    {
        numberPerDay[fish]++;
    }

    for (auto i = 0 ; i < days ; i++)
    {
        uint64 reproducers = numberPerDay[0];
        numberPerDay[0] = numberPerDay[1];
        numberPerDay[1] = numberPerDay[2];
        numberPerDay[2] = numberPerDay[3];
        numberPerDay[3] = numberPerDay[4];
        numberPerDay[4] = numberPerDay[5];
        numberPerDay[5] = numberPerDay[6];
        numberPerDay[6] = numberPerDay[7] + reproducers;
        numberPerDay[7] = numberPerDay[8];
        numberPerDay[8] = reproducers;
    }

    uint64 count = 0;
    for (auto number: numberPerDay) count += number;

    return to_string(count);
}

string computePartB( string fileName, string param )
{
    return computePartA(fileName, param);
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
