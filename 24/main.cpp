#include <regex>
#include <set>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;

vector<Test> partATests =
{
    /*
    {"test1.txt", "5", "-5"},
    {"test2.txt", "26", "1"},
    {"test2.txt", "25", "0"},
    {"test2.txt", "7", "1"},
    {"test2.txt", "6", "0"},
    */
};
string paramA = ""; 
 
vector<Test> partBTests =
{
    {"test3.txt", "", "2758514936282235"},
};
string paramB = "";

enum Operations
{
    INP,
    ADD,
    MUL,
    DIV,
    MOD,
    EQL
}; 

struct Instruction
{
    Operations op;
    char       reg1;
    char       reg2;
    int        val2;
    bool       isArg2reg;
};

string computePartA( string fileName, string param )
{
    // Read the data
    auto lines = readLines( fileName );

    regex patt( R"((...) (.) (.+)?)" );
    smatch match;

    vector<Instruction> program( lines.size() );

    int count = 0;
    for ( auto& line : lines )
    {
        auto& instruct = program[ count ];

        auto success = regex_match( line, match, patt );

        if ( match[ 1 ] == "inp") instruct.op = INP;
        if ( match[ 1 ] == "add") instruct.op = ADD;
        if ( match[ 1 ] == "mul") instruct.op = MUL;
        if ( match[ 1 ] == "div") instruct.op = DIV;
        if ( match[ 1 ] == "mod") instruct.op = MOD;
        if ( match[ 1 ] == "eql") instruct.op = EQL;

        instruct.reg1 = match[ 2 ].str()[ 0 ];

        if ( isalpha( match[ 2 ].str()[0] ) )
        {
            instruct.reg2 = match[ 2 ].str()[ 0 ];
            instruct.isArg2reg = true;
        }
        else
        {
            instruct.val2 = atoi( match[ 2 ].str().c_str() );
            instruct.isArg2reg = false;
        }
    }

    return to_string( 0 );
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
        //runPart("B", computePartB, partBTests, paramB);
    }
    else if (string(argv[1]) == "A")
    {
        runPart("A", computePartA, partATests, paramA);
    }
    else if (string(argv[1]) == "B")
    {
        //runPart("B", computePartB, partBTests, paramB);
    }
}
