#include <cassert> 

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;

vector<Test> partATests =
{
    {"test1.txt", "", "4140"},
};
string paramA = ""; 
 
vector<Test> partBTests =
{
    {"test1.txt", "", ""},
};
string paramB = "";

struct Node
{
    int leftVal;
    Node *leftNode;
    int rightVal;
    Node *rightNode;

    Node()
    {
        leftVal  = rightVal = 0;
        leftNode = rightNode = nullptr;
    }

    void setLeft( int val )
    {
        leftVal  = val;
        leftNode = nullptr;
    }

    void setLeft( Node *val )
    {
        leftVal  = 0;
        leftNode = val;
    }

    void setRight( int val )
    {
        rightVal  = val;
        rightNode = nullptr;
    }

    void setRight( Node *val )
    {
        rightVal  = 0;
        rightNode = val;
    }
};

Node *parsePair( string &str, int &pos );

string computePartA( string fileName, string param )
{
    auto lines = readLines( fileName );

    Node *sum;
    for ( auto &line: lines )
    {
        int pos = 0;
        Node *number = parsePair( line, pos );
    }
    return to_string( 0 );
}

Node *parsePair( string &str, int &pos )
{
    Node *n = new Node;
    
    // Consume the left bracket.
    if ( str[ pos++ ] != '[' ) printf( "Error: Expected '['\n" );

    // Parse the left term
    if ( str[ pos ] == '[') 
        n->leftNode = parsePair( str, pos );
    else 
        n->leftVal = str[ pos++ ] - '0';
    
    // Parse the comma
    if ( str[ pos++ ] != ',' ) printf( "Error: Expected ','\n" );
    
    // Parse the right term
    if ( str[ pos ] == '[') 
        n->rightNode = parsePair( str, pos );
    else 
        n->rightVal = str[ pos++ ] - '0';
        
    // Consume the right bracket.
    if ( str[ pos++ ] != ']' ) printf( "Error: Expected ']'\n" );

    return n;
}


string computePartB( string fileName, string param )
{
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
