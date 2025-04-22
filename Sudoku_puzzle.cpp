#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <utility>

using namespace std;

const int N = 9;
typedef vector<vector<int>> Grid;
Grid given;                                             
vector<vector<bool>> isFixed(N, vector<bool>(N, false)); 

void printBoard(const Grid &board)
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
            cout << board[i][j] << " ";
        cout << "\n";
    }
}

// Heuristic function: number of row and column conflicts
int getConflicts(const Grid &board)
{
    int conflicts = 0;
    // Row conflicts
    for (int i = 0; i < N; ++i)
    {
        vector<int> count(N + 1, 0);
        for (int j = 0; j < N; ++j)
            count[board[i][j]]++;
        for (int k = 1; k <= N; ++k)
            if (count[k] > 1)
                conflicts += count[k] - 1;
    }
    // Column conflicts
    for (int j = 0; j < N; ++j)
    {
        vector<int> count(N + 1, 0);
        for (int i = 0; i < N; ++i)
            count[board[i][j]]++;
        for (int k = 1; k <= N; ++k)
            if (count[k] > 1)
                conflicts += count[k] - 1;
    }
    return conflicts;
}

// Fill 3x3 blocks randomly (respecting original values)
void fillRandom(Grid &board)
{
    for (int br = 0; br < 3; ++br)
    {
        for (int bc = 0; bc < 3; ++bc)
        {
            vector<int> nums;
            vector<pair<int, int>> empty;
            vector<bool> used(N + 1, false);

            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    int r = br * 3 + i, c = bc * 3 + j;
                    if (given[r][c] != 0)
                    {
                        board[r][c] = given[r][c];
                        used[given[r][c]] = true;
                    }
                    else
                    {
                        empty.push_back(make_pair(r, c));
                    }
                }
            }

            for (int k = 1; k <= N; ++k)
                if (!used[k])
                    nums.push_back(k);

            random_shuffle(nums.begin(), nums.end());

            for (size_t i = 0; i < empty.size(); ++i)
                board[empty[i].first][empty[i].second] = nums[i];
        }
    }
}

// Hill Climbing Search
bool hillClimb(Grid &board, int maxIterations = 1000)
{
    int g = 0;
    int h = getConflicts(board);
    int f = g + h;

    cout << "Step " << g << " => g(n): " << g << ", h(n): " << h << ", f(n): " << f << endl;

    int iterationCount = 0;

    while (iterationCount < maxIterations)
    {
        bool improved = false;

        for (int br = 0; br < 3 && !improved; ++br)
        {
            for (int bc = 0; bc < 3 && !improved; ++bc)
            {
                vector<pair<int, int>> candidates;

                for (int i = 0; i < 3; ++i)
                {
                    for (int j = 0; j < 3; ++j)
                    {
                        int r = br * 3 + i;
                        int c = bc * 3 + j;
                        if (!isFixed[r][c])
                            candidates.push_back(make_pair(r, c));
                    }
                }

                for (size_t i = 0; i < candidates.size() && !improved; ++i)
                {
                    for (size_t j = i + 1; j < candidates.size(); ++j)
                    {
                        int r1 = candidates[i].first, c1 = candidates[i].second;
                        int r2 = candidates[j].first, c2 = candidates[j].second;

                        swap(board[r1][c1], board[r2][c2]);
                        int newH = getConflicts(board);

                        if (newH < h)
                        {
                            g++;
                            h = newH;
                            f = g + h;
                            cout << "Step " << g << " => g(n): " << g << ", h(n): " << h << ", f(n): " << f << endl;
                            improved = true;
                            break;
                        }
                        else
                        {
                            swap(board[r1][c1], board[r2][c2]);
                        }
                    }
                }
            }
        }

        if (!improved)
            break;

        iterationCount++;
    }

    cout << "Hill climbing terminated after " << iterationCount << " iterations." << endl;

    return (h == 0);
}

bool solveSudoku(Grid &board, int maxTries = 10000)
{
    srand(static_cast<unsigned>(time(0)));
    for (int attempt = 0; attempt < maxTries; ++attempt)
    {
        fillRandom(board);
        if (hillClimb(board))
            return true;
    }
    return false;
}

int main()
{
    given = {
        {5, 3, 0, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {0, 9, 8, 3, 4, 2, 0, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 0, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    // Initialize isFixed matrix
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            isFixed[i][j] = (given[i][j] != 0);

    Grid board = given;
    if (solveSudoku(board))
    {
        cout << "Solved Sudoku:\n";
        printBoard(board);
    }
    else
    {
        cout << "Failed to solve.\n";
    }

    return 0;
}
