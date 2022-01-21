#include <regex>

#include "../common/testing.h"
#include "../common/readers.h"
#include "../common/util.h"

using namespace std;

vector<Test> partATests =
{
    {"test1.txt", "", "739785"},
};
string paramA = ""; 
 
vector<Test> partBTests =
{
    {"test1.txt", "", ""},
};
string paramB = "";

struct Die
{
    int value = 0;
    int rolls = 0;

    int roll()
    {
        rolls++;
        value++;
        if (value > 100) value = 1;
        return value;
    }
};

string computePartA( string fileName, string param )
{
    // Read the data
    auto lines = readLines( fileName );

    // Variables to hold positions and scores
    int pos[2];
    int score[2] = {0};

    // Read the initial positions
    regex posPattern( R"(Player [12] starting position: (\d+))" );
    smatch match;
    regex_match( lines[0], match, posPattern );
    pos[0] = stoi(match[1]);

    regex_match( lines[1], match, posPattern );
    pos[1] = stoi(match[1]);

    Die die;
    // Play the game
    bool done = false;
    while ( !done )
    {
        for ( int i = 0 ; i < 2 ; i++ )
        {
            int total = die.roll() + die.roll() + die.roll();
            total %= 10;
            pos[i] += total;
            pos[i] = (pos[i] - 1) % 10 + 1;
            score[i] += pos[i];
            if (score[i] >= 1000)
            {
                done = true;
                break;
            }
        }
    }

    int result = min(score[0], score[1]) * die.rolls;

    return to_string( result );
}

typedef long long int64;

// Arrays to the count of each game state.
// Each game has four dimensions:
// Player 1: {position & score}
// Player 2: {position & score}
// The below matrix is configured as 
// ways[pos1][pos2][score1][score2]
int64 gamesBeforeMoves[11][11][21][21];
int64 gamesAfterMoves[11][11][21][21];

int64 getTotalGames()
{
    int64 total = 0;
    for ( int pos1 = 1 ; pos1 <= 10 ; pos1++ )
    for ( int pos2 = 1 ; pos2 <= 10 ; pos2++ )
    for ( int score1 = 0 ; score1 < 21 ; score1++ )
    for ( int score2 = 0 ; score2 < 21 ; score2++ )
        total += gamesBeforeMoves[pos1][pos2][score1][score2];
    return total;
}

string computePartB( string fileName, string param )
{
    // Read the data
    auto lines = readLines( fileName );

    // Read the initial positions
    regex posPattern( R"(Player [12] starting position: (\d+))" );
    smatch match;
    regex_match( lines[0], match, posPattern );
    int pos1 = stoi(match[1]);

    regex_match( lines[1], match, posPattern );
    int pos2 = stoi(match[1]);

    // Clear out and initialize the game matrix.
    int N = sizeof(gamesAfterMoves);
    memset(gamesBeforeMoves, 0, sizeof(gamesBeforeMoves));
    memset(gamesAfterMoves, 0, sizeof(gamesAfterMoves));
    gamesBeforeMoves[pos1][pos2][0][0] = 1;

    // Compute the different ways that three rolls could total
    int totals[10] = {0};
    for ( int i = 1 ; i <= 3 ; i++ )
        for ( int j = 1 ; j <= 3 ; j++ )
            for ( int k = 1 ; k <= 3 ; k++ )
            {
                int total = i + j + k;
                totals[total]++;
            }

    int64 player1wins = 0;
    int64 player2wins = 0;

    // Play the game
    bool done = false;
    int turn = 1;
    while ( !done )
    {
        done = true;

        // Player 1's turn
        for ( pos1 = 1 ; pos1 <= 10 ; pos1++ )
        for ( pos2 = 1 ; pos2 <= 10 ; pos2++ )
        for ( int score1 = 0 ; score1 < 21 ; score1++ )
        for ( int score2 = 0 ; score2 < 21 ; score2++ )
        {
            int n = gamesBeforeMoves[pos1][pos2][score1][score2];
            if (n == 0) continue;
            done = false;

            // Iterate through all possible rolls
            for ( int r1 = 1 ; r1 <= 3 ; r1++ )
            for ( int r2 = 1 ; r2 <= 3 ; r2++ )
            for ( int r3 = 1 ; r3 <= 3 ; r3++ )
            {
                int total = r1 + r2 + r3;
                int newPos1 = pos1 + total;
                if (newPos1 > 10) newPos1 -= 10;
                int newScore1 = score1 + newPos1;
                if (newScore1 >= 21)
                {
                    player1wins += n;
                }
                else
                {
                    gamesAfterMoves[newPos1][pos2][newScore1][score2] += n;
                }
            }
        }

        // Copy and reset the game buffers
        memcpy(gamesBeforeMoves, gamesAfterMoves, sizeof(gamesAfterMoves));
        memset(gamesAfterMoves, 0, sizeof(gamesAfterMoves));

        // Player 2's turn
        for ( pos1 = 1 ; pos1 <= 10 ; pos1++ )
        for ( pos2 = 1 ; pos2 <= 10 ; pos2++ )
        for ( int score1 = 0 ; score1 < 21 ; score1++ )
        for ( int score2 = 0 ; score2 < 21 ; score2++ )
        {
            int n = gamesBeforeMoves[pos1][pos2][score1][score2];
            if (n == 0) continue;
            done = false;

            // Iterate through all possible rolls
            for ( int r1 = 1 ; r1 <= 3 ; r1++ )
            for ( int r2 = 1 ; r2 <= 3 ; r2++ )
            for ( int r3 = 1 ; r3 <= 3 ; r3++ )
            {
                int total = r1 + r2 + r3;
                int newPos2 = pos2 + total;
                if (newPos2 > 10) newPos2 -= 10;
                int newScore2 = score2 + newPos2;
                if (newScore2 >= 21)
                {
                    player2wins += n;
                }
                else
                {
                    gamesAfterMoves[pos1][newPos2][score1][newScore2] += n;
                }
            }
        }

        // Copy and reset the game buffers
        memcpy(gamesBeforeMoves, gamesAfterMoves, sizeof(gamesAfterMoves));
        memset(gamesAfterMoves, 0, sizeof(gamesAfterMoves));

        int64 totalGames = getTotalGames();

        printf("\n%d: Total Games: %I64d,  Wins: %I64d, %I64d\n", turn, totalGames, player1wins, player2wins);
        turn++;
    }

    int64 result = max(player1wins, player2wins);

    return to_string( result );
}
// 444,356,092,776,315
//   5,471,399,467,034
//   5,471,399,466,980
//   8,451,884,679,112
//  25,742,810,304,379


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
