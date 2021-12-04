
#include "../common/testing.h"

using namespace std;

vector<Test> partATests =
{
    {"test.txt", "", "150"} 
};
 
vector<Test> partBTests =
{
    {"test.txt", "", "900"} 
};

string computePartA( string fileName, string param )
{
    auto lines = readLines( fileName );
    int N = lines.size();

    int x = 0;
    int y = 0;
    for (auto line : lines)
    {
        auto idx = line.find(" ");
        auto command = line.substr(0,idx);
        auto amount = stoi(line.substr(idx+1));

        if (command == "down")    y += amount;
        if (command == "up")      y -= amount;
        if (command == "forward") x += amount;
    }
    
    auto prod = x * y;
    return to_string(prod);
}

string computePartB( string fileName, string param )
{
    auto lines = readLines( fileName );
    int N = lines.size();

    int x = 0;
    int y = 0;
    int aim = 0;
    for (auto line : lines)
    {
        auto idx = line.find(" ");
        auto command = line.substr(0,idx);
        auto amount = stoi(line.substr(idx+1));

        if (command == "down") aim += amount;
        if (command == "up") aim -= amount;
        if (command == "forward") 
        {
            x += amount;
            y += aim * amount;
        }
    }
    
    auto prod = x * y;
    return to_string(prod);
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        runPart("A", computePartA, partATests);
        runPart("B", computePartB, partBTests);
    }
    else if (string(argv[1]) == "A")
    {
        runPart("A", computePartA, partATests);
    }
    else if (string(argv[1]) == "B")
    {
        runPart("B", computePartB, partBTests);
    }
}
