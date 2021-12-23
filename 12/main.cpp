#include <map>
#include <unordered_map>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;


vector<Test> partATests =
{
    {"test1.txt", "1", "10"},
    {"test2.txt", "1", "19"},
    {"test3.txt", "1", "226"}
};
string paramA = "1"; 
 
vector<Test> partBTests =
{
    {"test1.txt", "2", "36"},
    {"test2.txt", "2", "103"},
    {"test3.txt", "2", "3509"}
};
string paramB = "2";

map<string, int> visits;
vector< pair<string, string> > edges;
int visitNode( string node );
vector<string> getNeighbors( string node );
bool anyVisitedTwice();
vector<string> path;

int maxVisits;

string computePartA( string fileName, string param )
{
    visits.clear();
    edges.clear();
    path.clear();

    maxVisits = stoi(param);
    auto lines = readLines(fileName);
    for (auto line : lines)
    {
        auto dash = line.find('-');
        auto v1 = line.substr(0,dash);
        auto v2 = line.substr(dash+1);
        visits[v1] = 0;
        visits[v2] = 0;
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
        // for (auto &p : path) cout << p << ",";
        // cout << "end" << endl;
        return 1;
    }
    int thisMaxVisits = anyVisitedTwice() ? 1 : maxVisits;
    if ( visits[node] == thisMaxVisits ) return 0;
    if ( islower(node[0]) ) visits[node]++;

    path.push_back(node);
    auto neighbors = getNeighbors( node );
    int count = 0;
    for ( auto &n : neighbors )
    {
        count += visitNode( n );
    }
    if ( islower(node[0]) ) visits[node]--;
    path.pop_back();
    return count;
}

bool anyVisitedTwice()
{
    for ( auto &v: visits )
    {
        if ( v.second == 2) return true;
    }
    return false;
}

vector<string> getNeighbors( string node )
{
    vector<string> result;
    for (auto &e : edges)
    {   
        string next;
        if ( e.first  == node) next = e.second;
        if ( e.second == node) next = e.first;
        if ( next == "" ) continue;

        if ( visits[ next ] == maxVisits) continue;
        if ( next == "start") continue;
        result.push_back( next );
    }
    return result;
}


string computePartB( string fileName, string param )
{
    return computePartA( fileName, param );
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
