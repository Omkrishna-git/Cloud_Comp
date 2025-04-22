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


This code implements a **Hill Climbing algorithm** to solve a Sudoku puzzle. Hill climbing is an optimization algorithm that starts with a random solution and iteratively moves toward better solutions. It stops when it reaches a solution that is as good as it can get, given the constraints.

Let's walk through the code step-by-step and explain how it works, using an example.

---

### Key Components of the Code:

1. **Data Structures**:
   - `Grid`: A 2D vector (9x9 grid) used to represent the Sudoku puzzle.
   - `given`: Holds the initial state of the Sudoku puzzle (with some empty cells as 0).
   - `isFixed`: A 2D vector that tracks which cells are fixed (i.e., have a non-zero value in the initial grid).

2. **Functions**:
   - `printBoard()`: Displays the Sudoku grid in a readable format.
   - `getConflicts()`: Calculates the number of row and column conflicts in the grid.
   - `fillRandom()`: Randomly fills the 3x3 blocks while respecting the fixed values from the original puzzle.
   - `hillClimb()`: Performs the Hill Climbing search to optimize the Sudoku grid.
   - `solveSudoku()`: Tries to solve the Sudoku by filling the grid and applying Hill Climbing until it finds a valid solution.

---

### Step-by-Step Explanation:

#### Step 1: The `main()` function

In the `main()` function, the following happens:
- **Initialization**: A 9x9 Sudoku grid (`given`) is initialized with some fixed values and some empty cells (marked as `0`).
  
  Example of the initial grid:

  ```
  5 3 0 6 7 8 9 1 2
  6 7 2 1 9 5 3 4 8
  0 9 8 3 4 2 0 6 7
  8 5 9 7 6 1 4 2 3
  4 2 6 8 5 3 7 9 1
  7 1 3 9 2 4 8 5 6
  9 6 1 0 3 7 2 8 4
  2 8 7 4 1 9 6 3 5
  0 0 0 0 0 0 0 0 0
  ```

  Here, the empty cells are represented by `0`.

- **Fixed Cells**: The `isFixed` matrix is created to track which cells are fixed (non-zero). This helps the algorithm know which cells cannot be modified during the optimization process.
  
  For example:
  ```cpp
  isFixed[i][j] = (given[i][j] != 0);
  ```

  This will mark all the cells with values `1-9` as fixed and `0` as variable (modifiable).

#### Step 2: Filling the Grid Randomly (`fillRandom()`)

The `fillRandom()` function fills the 3x3 blocks of the grid while respecting the initial values provided by `given`. The idea here is to ensure that the initial, non-zero values remain fixed, while the empty cells are filled randomly with the remaining numbers.

- **For each 3x3 block**:
  - It first collects all the fixed values and marks them as used.
  - Then, it adds the missing values (from 1 to 9) to a list of possible numbers and shuffles them.
  - Finally, the remaining empty cells in the 3x3 block are filled with these shuffled numbers.

This helps generate a random, valid Sudoku board that respects the fixed values.

#### Step 3: The Hill Climbing Algorithm (`hillClimb()`)

The `hillClimb()` function attempts to solve the puzzle by iteratively improving the grid. Here’s how it works:

- **Initial Setup**: 
  - `g`: The number of moves made so far.
  - `h`: The number of conflicts in the board.
  - `f = g + h`: The overall cost function.

- **Main Loop**:
  The algorithm runs for up to `maxIterations` (1000 by default). In each iteration, it attempts to improve the grid:
  
  - **Finding Candidates for Swap**: For each 3x3 block in the grid, it identifies cells that are **not fixed** (i.e., cells that can be modified).
  
  - **Swapping Cells**: For every pair of non-fixed cells, it swaps their values and calculates the new conflict score (`newH`). If the new configuration reduces the number of conflicts (`newH < h`), the swap is accepted (i.e., the board is updated), and the number of conflicts (`h`) is updated.

  - **Stopping Condition**: If no improvement is found in a whole iteration, the algorithm terminates. If it finds a configuration with zero conflicts (`h == 0`), it means the puzzle is solved.

- **Goal**: The goal is to reduce the number of conflicts, which is measured by how many numbers repeat in the rows or columns.

#### Step 4: Trying to Solve the Puzzle (`solveSudoku()`)

The `solveSudoku()` function attempts to solve the puzzle by:
- **Randomly Filling the Board**: It first calls `fillRandom(board)` to generate a random configuration that respects the fixed values.
- **Hill Climbing**: Then, it runs the `hillClimb()` function to improve the configuration by swapping cells.

This process is repeated for a maximum of `maxTries` attempts (10,000 by default). If a solution is found (i.e., `hillClimb()` returns `true`), the solved Sudoku board is printed. Otherwise, it prints "Failed to solve."

---

### Example Walkthrough:

Given the initial puzzle:

```
5 3 0 6 7 8 9 1 2
6 7 2 1 9 5 3 4 8
0 9 8 3 4 2 0 6 7
8 5 9 7 6 1 4 2 3
4 2 6 8 5 3 7 9 1
7 1 3 9 2 4 8 5 6
9 6 1 0 3 7 2 8 4
2 8 7 4 1 9 6 3 5
0 0 0 0 0 0 0 0 0
```

The process would be:
1. **Randomly Fill Blocks**: The empty cells (0s) will be filled randomly while respecting the fixed values.
   
   Example after `fillRandom()`:

   ```
   5 3 4 6 7 8 9 1 2
   6 7 2 1 9 5 3 4 8
   1 9 8 3 4 2 5 6 7
   8 5 9 7 6 1 4 2 3
   4 2 6 8 5 3 7 9 1
   7 1 3 9 2 4 8 5 6
   9 6 1 5 3 7 2 8 4
   2 8 7 4 1 9 6 3 5
   3 4 5 2 8 6 1 7 9
   ```

2. **Hill Climbing**: The algorithm will start with this random board and try to reduce the number of conflicts (repeated numbers in rows/columns) by swapping cells.

3. The process will iterate, and eventually, the algorithm may find a board with zero conflicts, i.e., a solved Sudoku puzzle.

---

### Conclusion:

The Hill Climbing approach used here is a heuristic search that attempts to solve the Sudoku puzzle by iterating through potential solutions, evaluating their "fitness" (number of conflicts), and improving them by making small changes (cell swaps). The algorithm continues until either a solution is found or the maximum number of iterations is reached.
