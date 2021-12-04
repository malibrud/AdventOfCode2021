
#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;

vector<Test> partATests =
{
    {"test.txt", "", "4512"} 
};
 
vector<Test> partBTests =
{
    {"test.txt", "", "1924"} 
};

struct Board
{
    static const int MARKED = -1;
    bool won = false;
    int grid[5][5];

    void scoreDraw(int draw)
    {
        for (auto row = 0 ; row < 5 ; row++)
        {
            for (auto col = 0 ; col < 5 ; col++)
            {
                if (grid[row][col] == draw)
                    grid[row][col] = MARKED;
            }
        }
    }

    bool isWinner()
    {
        if (won) return true;

        // Check rows
        for (auto row = 0 ; row < 5 ; row++)
        {
            bool winner = true;
            for (auto col = 0 ; col < 5 ; col++)
            {
                winner &= grid[row][col] == MARKED;
            }
            if (winner) 
            {
                won = true;
                return true;
            }
        }

        // Check columns
        for (auto col = 0 ; col < 5 ; col++)
        {
            bool winner = true;
            for (auto row = 0 ; row < 5 ; row++)
            {
                winner &= grid[row][col] == MARKED;
            }
            if (winner)
            {
                won = true;
                return true;
            }
        }

        return false;
    }
    
    int getSumUnmarked()
    {
        int sum = 0;
        for (auto row = 0 ; row < 5 ; row++)
        {
            for (auto col = 0 ; col < 5 ; col++)
            {
                if (grid[row][col] != MARKED)
                    sum += grid[row][col] ;
            }
        }
        return sum;
    }
};

vector<Board> readBoards(vector<string> &lines);
int countWinners(vector<Board> &boards);

string computePartA( string fileName, string param )
{
    auto lines = readLines( fileName );
    int nLines = lines.size();
    int nBoards = (nLines - 1) / 6;
    
    auto draws = parseCsvInts( lines[0] );
    auto boards = readBoards( lines );

    auto winningScore = 0;
    for (auto draw : draws)
    {
        for (auto &board : boards)
        {
            board.scoreDraw(draw);
            if (board.isWinner())
            {
                winningScore = draw * board.getSumUnmarked();
                goto WINNER_FOUND;
            }
        }
    }
WINNER_FOUND:
    return to_string(winningScore);
}

string computePartB( string fileName, string param )
{
    auto lines = readLines( fileName );
    int nLines = lines.size();
    int nBoards = (nLines - 1) / 6;
    
    auto draws = parseCsvInts( lines[0] );
    auto boards = readBoards( lines );

    auto winningScore = 0;
    for (auto draw : draws)
    {
        for (auto &board : boards)
        {
            board.scoreDraw(draw);
            if (countWinners(boards) == nBoards)
            {
                winningScore = draw * board.getSumUnmarked();
                goto WINNER_FOUND;
            }
        }
    }
WINNER_FOUND:
    return to_string(winningScore);
}

vector<Board> readBoards(vector<string> &lines) 
{
    int nLines = lines.size();
    int nBoards = (nLines - 1) / 6;

    vector<Board> boards;
    boards.reserve(nBoards);
    Board board;

    int lineIdx = 2;
    // Read all boards
    for (auto i = 0 ; i < nBoards ; i++)
    {
        // Read rows
        for (auto row = 0 ; row < 5 ; row++)
        {
            auto line = lines[lineIdx++];
            // Read columns
            auto curpos = 0;
            for (auto col = 0 ; col < 5 ; col++)
            {
                string field;
                if (line[curpos] == ' ')
                    field = line.substr(curpos+1, 1);  // single digit
                else
                    field = line.substr(curpos, 2);  // two digits

                board.grid[row][col] = stoi(field);
                curpos += 3;
            }
        }
        boards.push_back(board);
        lineIdx++;
    }
    return boards;
}

int countWinners(vector<Board> &boards)
{
    int count = 0;
    for (auto &board : boards)
    {
        count += board.isWinner() ? 1 : 0;
    }
    return count;
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
