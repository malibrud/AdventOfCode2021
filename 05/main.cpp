#include <regex>
#include <cmath>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;


struct Segment
{
    int x1, y1;
    int x2, y2;

    bool isHoriz()
    {
        return y1 == y2;
    }

    bool isVert()
    {
        return x1 == x2;
    }
    
    bool isDiagonal()
    {
        return !isHoriz() && !isVert();
    }

    int getSlope()
    {
        return (y2 - y1) / (x2 - x1);
    }

    inline int minX() { return x1 < x2 ? x1 : x2; }
    inline int maxX() { return x1 > x2 ? x1 : x2; }
    inline int minY() { return y1 < y2 ? y1 : y2; }
    inline int maxY() { return y1 > y2 ? y1 : y2; }
};

struct Field
{
    int N;
    unsigned short **grid;

    Field(int _N)
    {
        N = _N;
        grid = new unsigned short *[N];
        for (int i = 0 ; i < N ; i++)
        {
            grid[i] = new unsigned short[N];
            for (int j = 0 ; j < N ; j++)
            {
                grid[i][j] = 0;
            }
        }
    }

    ~Field()
    {
        for (int i = 0 ; i < N ; i++)
        {
            delete[] grid[i];
        }
        delete[] grid;

    }

    void processSegments(vector<Segment> &segments)
    {
        for (auto &seg : segments)
        {
            if (seg.isHoriz())
            {
                auto y = seg.y1;
                for (auto x = seg.minX() ; x <= seg.maxX() ; x++) grid[y][x]++;
            }

            if (seg.isVert())
            {
                auto x = seg.x1;
                for (auto y = seg.minY() ; y <= seg.maxY() ; y++) grid[y][x]++;
            }
        }
    }

    void processSegmentsWithDiagonal(vector<Segment> &segments)
    {
        processSegments(segments);
        for (auto &seg : segments)
        {
            if (seg.isDiagonal())
            {
                auto slope = seg.getSlope();
                if (seg.x1 < seg.x2)
                {
                    for (auto x = seg.x1 ; x <= seg.x2 ; x++) 
                    {
                        auto y = (x - seg.x1) * slope + seg.y1;
                        grid[y][x]++;
                    }
                }
                if (seg.x2 < seg.x1)
                {
                    for (auto x = seg.x2 ; x <= seg.x1 ; x++) 
                    {
                        auto y = (x - seg.x2) * slope + seg.y2;
                        grid[y][x]++;
                    }
                }
            }
        }
    }

    int countOverlapPoints()
    {
        auto count = 0;
        for (auto i = 0 ; i < N ; i++)
            for (auto j = 0 ; j < N ; j++)
                if (grid[i][j] >= 2)
                    count ++;
        return count;
    }
};

vector<Segment> parseSegments(vector<string> &lines);
int getMaxCoordinate(vector<Segment> segments);

vector<Test> partATests =
{
    {"test.txt", "", "5"} 
};
 
vector<Test> partBTests =
{
    {"test.txt", "", "12"} 
};

string computePartA( string fileName, string param )
{
    auto lines = readLines(fileName);
    auto segments = parseSegments(lines);
    auto size = getMaxCoordinate(segments);
    Field field(size+1);
    field.processSegments(segments);
    auto nOverlap = field.countOverlapPoints();
    return to_string(nOverlap);
}

string computePartB( string fileName, string param )
{
    auto lines = readLines(fileName);
    auto segments = parseSegments(lines);
    auto size = getMaxCoordinate(segments);
    Field field(size+1);
    field.processSegmentsWithDiagonal(segments);
    auto nOverlap = field.countOverlapPoints();
    return to_string(nOverlap);
}

vector<Segment> parseSegments(vector<string> &lines) 
{
    vector<Segment> segments;
    regex segmentPattern( "(\\d+),(\\d+) -> (\\d+),(\\d+)" );

    smatch match;
    for (auto &line : lines)
    {
        Segment seg;
        regex_match(line, match, segmentPattern);
        seg.x1 = stoi(match[1]);
        seg.y1 = stoi(match[2]);
        seg.x2 = stoi(match[3]);
        seg.y2 = stoi(match[4]);
        segments.push_back(seg);
    }
    return segments;
}

int getMaxCoordinate(vector<Segment> segments)
{
    int maxVal = 0;
    for (auto &seg : segments)
    {
        maxVal = max(maxVal, seg.maxX());
        maxVal = max(maxVal, seg.maxY());
    }
    return maxVal;
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
