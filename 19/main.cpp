#include <vector>
#include <regex>
#include <algorithm>
#include <set>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;

vector<Test> partATests =
{
    {"test1.txt", "", "79"},
};
string paramA = ""; 
 
vector<Test> partBTests =
{
    {"test1.txt", "", "3993"},
};
string paramB = "";

struct Point
{
    int p[3];
    Point()
    {
        p[0] = p[1] = p[2] = 0;
    }

    Point operator+( Point& rhs )
    {
        Point sum;
        sum.p[0] = p[0] + rhs.p[0];
        sum.p[1] = p[1] + rhs.p[1];
        sum.p[2] = p[2] + rhs.p[2];
        return sum;
    }

    Point operator-( Point& rhs )
    {
        Point diff;
        diff.p[0] = p[0] - rhs.p[0];
        diff.p[1] = p[1] - rhs.p[1];
        diff.p[2] = p[2] - rhs.p[2];
        return diff;
    }

    bool operator==( Point& rhs )
    {
        return 
            p[0] == rhs.p[0] && 
            p[1] == rhs.p[1] && 
            p[2] == rhs.p[2]
        ;
    }

    bool operator<( const Point& q) const
    {
        if ( p[0] < q.p[0] ) return true;
        if ( p[0] > q.p[0] ) return false;
        if ( p[1] < q.p[1] ) return true;
        if ( p[1] > q.p[1] ) return false;
        if ( p[2] < q.p[2] ) return true;
        if ( p[2] > q.p[2] ) return false;
        return false;
    }

    void print()
    {
        printf( "[ %6d, %6d, %6d ]", p[0], p[1], p[2] );
    }
};

struct Rotation
{
    int R[3][3];

    Rotation()
    {
        for (int i = 0 ; i < 3 ; i++)
            for (int j = 0 ; j < 3 ; j++)
                R[i][j] = i == j ? 1 : 0;   // Identity
    }

    void print()
    {
        int i, j;
        for (i = 0 ; i < 3 ; i++)
        {
            printf( "[ %2d %2d %2d ]\n", R[i][0], R[i][1], R[i][2] );
        }
    }

    Point rotate( Point& p )
    {
        Point q;
        for (int i = 0 ; i < 3 ; i++ )
        {
            q.p[i] = 0;
            for ( int j = 0 ; j < 3 ; j++ )
            {
                q.p[i] += R[i][j] * p.p[j];
            }
        }
        return q;
    }

    Point operator*( Point& p)
    {
        Point q;
        for (int i = 0 ; i < 3 ; i++ )
        {
            q.p[i] = 0;
            for ( int j = 0 ; j < 3 ; j++ )
            {
                q.p[i] += R[i][j] * p.p[j];
            }
        }
        return q;
    }
    
    Rotation operator*( Rotation& Rr)
    {
        Rotation Q;
        for (int i = 0 ; i < 3 ; i++ )
        {
            for ( int j = 0 ; j < 3 ; j++ )
            {
                Q.R[i][j] = 0;
                for ( int k = 0 ; k < 3 ; k++ )
                {
                    Q.R[i][j] += R[i][k] * Rr.R[k][j];
                }
            }
        }
        return Q;
    }
};


struct Scan
{
    int idx;
    vector<Point> lPoints;
    vector<Point> gPoints;
    // Frame w.r.t. parent frame
    Rotation R;
    Point    T;
    // Frame w.r.t. global frame
    Rotation Rg;
    Point    Tg;
    bool     matched = false;
    int      match;

    int size() { return lPoints.size(); }

    void transform()
    {
        int N = lPoints.size();

        for ( int i = 0 ; i < N ; i++ )
        {
            gPoints[i] = Rg*lPoints[i] + Tg;
        }
    }
    
    bool localContains( Point& q )
    {
        for ( auto& p: lPoints )
        {
            if ( p == q ) return true;
        }
        return false;
    }
};


vector<Rotation> generateAllRotations();
vector< vector<string> > groupByScanner( vector<string> lines );
vector<Scan> parseScans( vector<vector<string>> scanGroups );
bool findMatches( vector<Scan>& scans, int upstreamIdx );
bool tryMatch( Scan& s1, Scan& s2 );
int countMatchesGivenOffset( Scan& s1, Scan& s2, Point& T );
int countBeacons( vector<Scan>& scans );

vector<Rotation> allRotations = generateAllRotations();

string computePartA( string fileName, string param )
{
    auto lines = readLines( fileName );
    auto scanGroups = groupByScanner( lines );
    auto scans = parseScans( scanGroups );

    auto N = scans.size();

    // Grow a list of matched scans
    vector<int> matchedList;

    // Setup the first scan as the global frame;
    scans[0].matched = true;
    scans[0].gPoints = scans[0].lPoints;
    
    findMatches( scans, 0 );

    int count = countBeacons( scans );

    return to_string( count );
}

string computePartB( string fileName, string param )
{
    return to_string( 0 );
}

vector< vector<string> > groupByScanner( vector<string> lines )
{
    vector< vector<string> > scans;
    vector<string> coords;

    smatch match;

    for ( auto &line: lines )
    {
        if ( line.size() == 0 )
        {
            scans.push_back( coords );
            continue;
        }
        if ( line.substr( 0, 3 ) == "---" )
        {
            coords.clear();
            continue;
        }
        coords.push_back( line );
    }
    scans.push_back( coords );

    return scans;
}

vector<Scan> parseScans( vector<vector<string>> scanGroups )
{
    vector<Scan> scans;
    regex coordPattern( R"((-?\d+),(-?\d+),(-?\d+))" );
    int i = 0;
    for ( auto scanGroup : scanGroups )
    {
        Scan scan;
        scan.lPoints.clear();
        scan.gPoints.clear();

        for ( auto scanLine : scanGroup )
        {
            Point p;
            smatch match;
            if ( regex_match( scanLine, match, coordPattern ) )
            {
                p.p[0] = stoi( match[1] );
                p.p[1] = stoi( match[2] );
                p.p[2] = stoi( match[3] );
                scan.lPoints.push_back( p );
            }
            else
            {
                throw exception("Could not parse coordinate.");
            }
        }
        //sort( scan.lPoints.begin(), scan.lPoints.end() );
        scan.gPoints = scan.lPoints;
        scan.idx = i++;
        scans.push_back( scan );
    }
    return scans;
}

bool findMatches( vector<Scan>& scans, int upstreamIdx )
{
    bool result = false;
    auto& upstream = scans[upstreamIdx];
    for ( int i = 0 ; i < scans.size() ; i++ )
    {
        auto& downstream = scans[i];
        if ( downstream.matched ) continue;
        if ( tryMatch( upstream, downstream ) )
        {
            printf( "Match: %d --> %d\n", upstream.idx, downstream.idx );
            findMatches( scans, i) ;
            result = true;
        }
    }
    return result;
}

bool tryMatch( Scan& s1, Scan& s2 )
{
    int N1 = s1.size();
    int N2 = s2.size();
    for ( auto& R : allRotations )
    {
        s2.Rg = R;
        s2.transform();
        for ( int i = 0 ; i < N1 ; i++ )
        {
            auto& p1 = s1.lPoints[i];
            for ( int j = 0 ; j < N2 ; j++ )
            {
                auto& p2 = s2.gPoints[j];
                Point T = p1 - p2;
                auto matches = countMatchesGivenOffset( s1, s2, T );
                if ( matches >= 12)
                {
                    s2.matched = true;
                    // Transform back to the global frame
                    s2.R = R;
                    s2.T = T;
                    s2.Rg = s1.Rg * R;
                    s2.Tg = s1.Rg * T + s1.Tg;
                    s2.transform();
                    s2.match = i;
                    s2.matched = true;
                    return true;
                }
            }
        }
    }
    return false;
}

int countMatchesGivenOffset( Scan& s1, Scan& s2, Point& T )
{
    int N1 = s1.size();
    int N2 = s2.size();

    int count = 0;
    for ( int i = 0 ; i < N2 ; i++ )
    {
        Point p2 = s2.gPoints[i] + T;
        if ( s1.localContains ( p2 ) ) count++;
    }
    return count;
}

int countBeacons( vector<Scan>& scans )
{
    set<Point> beacons;
    for ( auto& scan: scans )
    {
        for ( auto& p: scan.gPoints )
        {
            beacons.insert( p );
        }
    }
    return beacons.size();
}

vector<Rotation> generateAllRotations()
{
    vector<Rotation> rotations;
    rotations.reserve(24);
    Rotation R;
    
    int v[2] = {-1, 1};
    int i, j, k;
    int ix, iy, iz;
    int sx, sy, sz;
    for (i = 0 ; i < 6 ; i++)
    {
        ix = i % 3;
        sx = i < 3 ? +1 : -1;
        R.R[0][(ix+0)%3] = sx;
        R.R[0][(ix+1)%3] = 0;
        R.R[0][(ix+2)%3] = 0;

        for (j = 0 ; j < 6 ; j++)
        {
            iy = j % 3;
            sy = j < 3 ? +1 : -1;

            if (iy == ix) continue;

            R.R[1][(iy+0)%3] = sy;
            R.R[1][(iy+1)%3] = 0;
            R.R[1][(iy+2)%3] = 0;
            
            for (k = 0 ; k < 6 ; k++)
            {
                iz = k % 3;
                sz = k < 3 ? +1 : -1;

                if (iz == ix || iz == iy) continue;
                
                R.R[2][(iz+0)%3] = sz;
                R.R[2][(iz+1)%3] = 0;
                R.R[2][(iz+2)%3] = 0;

                int det = 0;
                det += R.R[0][0] * ( R.R[1][1] * R.R[2][2] - R.R[1][2] * R.R[2][1] );
                det += R.R[0][1] * ( R.R[1][2] * R.R[2][0] - R.R[1][0] * R.R[2][2] );
                det += R.R[0][2] * ( R.R[1][0] * R.R[2][1] - R.R[1][1] * R.R[2][0] );
                if (det == +1) 
                {
                    R.print();
                    cout << endl;
                    rotations.push_back( R );
                }
            }
        }
    }
    return rotations;
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
