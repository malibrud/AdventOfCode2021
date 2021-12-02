#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Test
{
    string input;
    string param;
    string output;
};

void runPartA();
void runPartB();
void runPart(string part, string (*func)(string, string), vector<Test> tests);
string computePartA( string fileName, string param );
string computePartB( string fileName, string param );

vector<string> readLines( string fileName );
vector<int> readInts( string fileName );
bool runTests( vector<Test> tests, string (*func)(string, string) );


vector<Test> partATests =
{
    {"test.txt", "", "7"}
};

vector<Test> partBTests =
{
    {"test.txt", "", "5"}
};

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

string computePartA( string fileName, string param )
{
    auto v = readInts( fileName );
    int N = v.size();

    int inc = 0;
    for (size_t i = 0; i < N-1; i++)
    {
        if (v[i] < v[i+1]) inc++;
    }
    
    return to_string(inc);
}

string computePartB( string fileName, string param )
{
    auto v = readInts( fileName );
    int N = v.size();

    int inc = 0;
    for (size_t i = 0; i < N-3; i++)
    {
        auto s1 = v[i+0] + v[i+1] + v[i+2];
        auto s2 = v[i+1] + v[i+2] + v[i+3];
        if (s1 < s2) inc++;
    }
    
    return to_string(inc);
}

//// Boilerplate code

void runPart(string part, string (*func)(string, string), vector<Test> tests)
{
    printf("Running Part %s:\n", part.c_str());
    runTests(tests, func);

    auto answer = func("data.txt", "");
    printf("The answer is: %s\n", answer.c_str());
    printf("\n\n");
}

bool runTests( vector<Test> tests, string (*func)(string, string) )
{
    printf("Running tests...\n");
    bool allPass = true;
    for ( size_t i = 0; i < tests.size(); i++ )
    {
        Test &t = tests[i];
        printf("[%d] %s(%s) -> %s...   ", i, t.input.c_str(), t.param.c_str(), t.output.c_str());
        string result = func( t.input, t.param );
        if (result == t.output)
        {
            printf("Pass\n");
        }
        else
        {
            printf("Fail\n");
            allPass = false;
        }
    }
    return allPass;
}

vector<string> readLines( string fileName )
{
    vector<string> lines;

    ifstream f( fileName );
    if ( !f.is_open() ) return lines;

    string line;
    while ( getline( f, line ) )
    {
        lines.push_back( line );
    }
    return lines;
}

vector<int> readInts( string fileName )
{
    vector<string> lines = readLines( fileName );
    vector<int> v;
    for (auto line: lines)
    {
        auto val = stoi(line);
        v.push_back(val);
    }
    return v;
}