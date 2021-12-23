#include <map>
#include <unordered_map>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;


vector<Test> partATests =
{
    {"test1.txt", "", "10"},
    {"test2.txt", "", "19"},
    {"test3.txt", "", "226"}
};
string paramA = "1"; 
 
vector<Test> partBTests =
{
    {"test1.txt", "", "36"},
    {"test2.txt", "", "103"},
    {"test3.txt", "", "226"}
};
string paramB = "2";

map<string, bool> visited;
vector< pair<string, string> > edges;
int visitNode( string node );
vector<string> getNeighbors( string node );
vector<string> path;

int paramMaxVisits;

string computePartA( string fileName, string param )
{
    visited.clear();
    edges.clear();
    path.clear();

    auto lines = readLines(fileName);
    for (auto line : lines)
    {
        auto dash = line.find('-');
        auto v1 = line.substr(0,dash);
        auto v2 = line.substr(dash+1);
        visited[v1] = false;
        visited[v2] = false;
        pair<string, string> edge(v1, v2);
        edges.push_back(edge);
    }
    auto paths = visitNode("start");

    return to_string(paths);
}

int visitNode(string node)
{
    if ( node == "end" ) 
    {   
        // cout << "Path found: ";
        // for (auto &p : path) cout << p << "-";
        // cout << "end" << endl;
        return 1;
    }
    if ( visited[ node ] ) return 0;
    if ( islower(node[0]) ) visited[node] = true;

    path.push_back(node);
    auto neighbors = getNeighbors( node );
    int count = 0;
    for ( auto &n : neighbors )
    {
        count += visitNode( n );
    }
    visited[node] = false;
    path.pop_back();
    return count;
}

vector<string> getNeighbors( string node )
{
    vector<string> result;
    for (auto &e : edges)
    {
        if ( e.first == node && !visited[ e.second ] ) result.push_back( e.second );
        if ( e.second == node && !visited[ e.first ] ) result.push_back( e.first );
    }
    return result;
}


string computePartB( string fileName, string param )
{
    return to_string(0);
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
