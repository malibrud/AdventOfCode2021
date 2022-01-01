#include <vector>
#include <cassert>
#include <numeric>
#include <algorithm>

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
    {"test1B.txt", "", "3"},
    {"test2B.txt", "", "54"},
    {"test3B.txt", "", "7"},
    {"test4B.txt", "", "9"},
    {"test5B.txt", "", "1"},
    {"test6B.txt", "", "0"},
    {"test7B.txt", "", "0"},
    {"test8B.txt", "", "1"}
};
string paramB = "";

typedef long long int64;
const int MIN_PACKET_BITS = 11;
int parsePacketsUpToIdx( vector<int> &bits, int pc, int end, vector<int64> &values );
int parseNpackets( vector<int> &bits, int pc, int N, vector<int64> &values );
int parsePacket( vector<int> &bits, int pc, int64 &outValue );
int parseLiteralPacket( vector<int> &bits, int pc, int64 &outValue );
int parseOperatorPacket( vector<int> &bits, int pc, int64 &outValue );
int parseNibble( vector<int> &bits, int pc, int64 &outNibble );
int parseNbitsAsInt( vector<int> &bits, int pc, int N, int64 &outVal );

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
    vector<int64> values;
    pc = parsePacketsUpToIdx( bits, pc, nBits, values );

    return to_string( versionSum );
}


string computePartB( string fileName, string param )
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
    int64 value;
    pc = parsePacket( bits, pc, value );

    return to_string( value );
}

int parsePacketsUpToIdx( vector<int> &bits, int pc, int end, vector<int64> &values )
{
    values.clear();
    while ( pc < end )
    {
        int64 value;
        pc = parsePacket( bits, pc, value );
        values.push_back( value );
    }
    return pc;
}

int parseNpackets( vector<int> &bits, int pc, int N, vector<int64> &values )
{
    values.clear();
    for ( int i = 0 ; i < N ; i++ )
    {
        int64 value;
        pc = parsePacket( bits, pc, value );
        values.push_back( value );
    }
    return pc;
}

int parsePacket( vector<int> &bits, int pc, int64 &outValue )
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
        pc = parseLiteralPacket( bits, pc, outValue );
        break;
    
    default: // Operator packet
        pc = parseOperatorPacket( bits, pc, outValue );
        break;
    }
    return pc;
}

int parseLiteralPacket( vector<int> &bits, int pc, int64 &outValue )
{
    outValue = 0;
    int64 nibble;
    while ( bits[ pc++ ] )
    {
        pc = parseNibble( bits, pc, nibble );
        outValue = ( outValue << 4 ) | nibble;
    }
    pc = parseNibble( bits, pc, nibble );
    outValue = ( outValue << 4 ) | nibble;

    return pc;
}

int parseOperatorPacket( vector<int> &bits, int pc, int64 &outValue )
{
    // Get the type from the packet header.
    int type = 0;
    type += bits[pc-3] << 2;
    type += bits[pc-2] << 1;
    type += bits[pc-1] << 0;

    // Get the sub-packets
    vector<int64> values;
    auto lengthTypeID = bits[ pc++ ];
    switch ( lengthTypeID )
    {
    case 0:
        int64 len;
        pc = parseNbitsAsInt( bits, pc, 15, len );
        pc = parsePacketsUpToIdx( bits, pc, pc+(int)len, values );
        break;

    case 1:
        int64 nPackets;
        pc = parseNbitsAsInt( bits, pc, 11, nPackets );
        pc = parseNpackets( bits, pc, (int)nPackets, values );
        break;

    default:
        break;
    }

    // Calculate the value of the operator packets
    switch ( type )
    {
    case 0: // sum
        outValue = 0;
        for (auto v: values) outValue += v;
        break;
    case 1: // product
        outValue = 1;
        for (auto v: values) outValue *= v;
        break;
    case 2: // minimum
        outValue = *min_element( values.begin(), values.end() );
        break;
    case 3: // maximum
        outValue = *max_element( values.begin(), values.end() );
        break;
    case 5: // greater than
        outValue = values[0] > values[1] ? 1 : 0;
        break;
    case 6: // less than
        outValue = values[0] < values[1] ? 1 : 0;
        break;
    case 7: // equal to
        outValue = values[0] == values[1] ? 1 : 0;
        break;
    default:
        break;
    }

    // Debugging prints
    // cout << "For vector: ";
    // for (auto v: values) cout << v << " ";
    // cout << "and operator " << type << endl;
    // cout << "    = " << outValue << endl;

    return pc;
}

int parseNibble( vector<int> &bits, int pc, int64 &outNibble )
{
    return parseNbitsAsInt( bits, pc, 4, outNibble );
}

int parseNbitsAsInt( vector<int> &bits, int pc, int N, int64 &outVal )
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
