#include <vector>
#include <cassert>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;

vector<Test> partATests =
{
    {"test1.txt", "", "6"},
    {"test2.txt", "", "9"},
    {"test3.txt", "", "14"},
    {"test4.txt", "", "16"},
    {"test5.txt", "", "12"},
    {"test6.txt", "", "23"},
    {"test7.txt", "", "31"}
};
string paramA = ""; 
 
vector<Test> partBTests =
{
    {"test1.txt", "", "315"}
};
string paramB = "";

const int MIN_PACKET_BITS = 11;
int parsePacketsUpToIdx( vector<int> &bits, int pc, int end );
int parseNpackets( vector<int> &bits, int pc, int N );
int parsePacket( vector<int> &bits, int pc );
int parseLiteralPacket( vector<int> &bits, int pc, int &outValue );
int parseOperatorPacket( vector<int> &bits, int pc, int &outValue );
int parseNibble( vector<int> &bits, int pc, int &outNibble );
int parseNbitsAsInt( vector<int> &bits, int pc, int N, int &outVal );

int versionSum = 0;
string computePartA( string fileName, string param )
{
    auto lines = readLines( fileName );
    auto hex = lines[0];
    auto nBits = 4 * hex.size();
    vector<int> bits( nBits, 0 );

    auto ibit = 0;
    for ( auto h: hex )
    {
        int nibble;
        if ( h <= '9') nibble = h - '0';
        else nibble = 10 + h - 'A';
        bits[ ibit++ ] = ( nibble & 8 ) >> 3;
        bits[ ibit++ ] = ( nibble & 4 ) >> 2;
        bits[ ibit++ ] = ( nibble & 2 ) >> 1;
        bits[ ibit++ ] = ( nibble & 1 ) >> 0;
    }

    auto pc = 0;
    versionSum = 0;
    pc = parsePacketsUpToIdx( bits, pc, nBits );

    return to_string( versionSum );
}

int parsePacketsUpToIdx( vector<int> &bits, int pc, int end )
{
    while ( pc < end )
    {
        pc = parsePacket( bits, pc );
    }
    return pc;
}

int parseNpackets( vector<int> &bits, int pc, int N )
{
    for ( int i = 0 ; i < N ; i++ )
    {
        pc = parsePacket( bits, pc );
    }
    return pc;
}

int parsePacket( vector<int> &bits, int pc )
{
    int end = bits.size();
    if ( end - pc < MIN_PACKET_BITS ) return end;

    int ver  = 0;
    int type = 0;

    ver += bits[pc++] << 2;
    ver += bits[pc++] << 1;
    ver += bits[pc++] << 0;
    versionSum += ver;

    type += bits[pc++] << 2;
    type += bits[pc++] << 1;
    type += bits[pc++] << 0;

    switch (type)
    {
    case 4: // Literal Value
        int outValue;
        pc = parseLiteralPacket( bits, pc, outValue );
        break;
    
    default: // Operator packet
        pc = parseOperatorPacket( bits, pc, outValue );
        break;
    }
    return pc;
}

int parseLiteralPacket( vector<int> &bits, int pc, int &outValue )
{
    outValue = 0;
    int nibble;
    while ( bits[ pc++ ] )
    {
        pc = parseNibble( bits, pc, nibble );
        outValue = ( outValue << 4 ) | nibble;
    }
    pc = parseNibble( bits, pc, nibble );
    outValue = ( outValue << 4 ) | nibble;

    return pc;
}

int parseOperatorPacket( vector<int> &bits, int pc, int &outValue )
{
    auto lengthTypeID = bits[ pc++ ];
    switch ( lengthTypeID )
    {
    case 0:
        int len;
        pc = parseNbitsAsInt( bits, pc, 15, len );
        pc = parsePacketsUpToIdx( bits, pc, pc+len );
        break;

    case 1:
        int nPackets;
        pc = parseNbitsAsInt( bits, pc, 11, nPackets );
        pc = parseNpackets( bits, pc, nPackets );
        break;

    default:
        break;
    }

    return pc;
}

int parseNibble( vector<int> &bits, int pc, int &outNibble )
{
    return parseNbitsAsInt( bits, pc, 4, outNibble );
}

int parseNbitsAsInt( vector<int> &bits, int pc, int N, int &outVal )
{
    outVal = 0;
    int shift = N-1;
    for ( auto i = 0 ; i < N ; i++ )
    {
        outVal |= bits[ pc++ ] << shift;
        shift--;
    }
    return pc;
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
