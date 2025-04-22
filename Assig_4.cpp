#include <iostream>
#include <vector>
using namespace std;

bool Solve(vector<vector<bool>> &board, int col);
void PlaceQueen(vector<vector<bool>> &board, int row, int col);
void RemoveQueen(vector<vector<bool>> &board, int row, int col);
bool RowIsClear(const vector<vector<bool>> &board, int queenRow, int queenCol);
bool UpperDiagIsClear(const vector<vector<bool>> &board, int queenRow, int queenCol);
bool LowerDiagIsClear(const vector<vector<bool>> &board, int queenRow, int queenCol);
bool IsSafe(const vector<vector<bool>> &board, int row, int col);
void ClearBoard(vector<vector<bool>> &board);
void PrintBoard(const vector<vector<bool>> &board);

int main() {
    int n;
    cout << "Enter size of Chess Board: " << endl;
    cin >> n;

    vector<vector<bool>> board(n, vector<bool>(n, false));

    if (Solve(board, 0)) {
        cout << "Solution : " << endl;
        PrintBoard(board);
    } else {
        cout << "No solution exists." << endl;
    }
    return 0;
}

// Function: Solve
bool Solve(vector<vector<bool>> &board, int col) {
    int n = board.size();
    if (col >= n) return true; // base case

    for (int row = 0; row < n; row++) {
        if (IsSafe(board, row, col)) {
            PlaceQueen(board, row, col); // try queen here

            if (Solve(board, col + 1)) return true; // recur to place rest

            RemoveQueen(board, row, col); // failed, remove, try again
        }
    }
    return false;
}

// Function: PlaceQueen
void PlaceQueen(vector<vector<bool>> &board, int row, int col) {
    board[row][col] = true;
}

// Function: RemoveQueen
void RemoveQueen(vector<vector<bool>> &board, int row, int col) {
    board[row][col] = false;
}

// Function: IsSafe
bool IsSafe(const vector<vector<bool>> &board, int row, int col) {
    return (LowerDiagIsClear(board, row, col) &&
            RowIsClear(board, row, col) &&
            UpperDiagIsClear(board, row, col));
}

// Function: RowIsClear
bool RowIsClear(const vector<vector<bool>> &board, int queenRow, int queenCol) {
    for (int col = 0; col < queenCol; col++) {
        if (board[queenRow][col]) return false;
    }
    return true;
}

// Function: UpperDiagIsClear
bool UpperDiagIsClear(const vector<vector<bool>> &board, int queenRow, int queenCol) {
    int row, col;
    int n = board.size();
    for (row = queenRow, col = queenCol; col >= 0 && row < n; row++, col--) {
        if (board[row][col]) return false;
    }
    return true;
}

// Function: LowerDiagIsClear
bool LowerDiagIsClear(const vector<vector<bool>> &board, int queenRow, int queenCol) {
    int row, col;
    for (row = queenRow, col = queenCol; row >= 0 && col >= 0; row--, col--) {
        if (board[row][col]) return false;
    }
    return true;
}

// Function: PrintBoard
void PrintBoard(const vector<vector<bool>> &board) {
    int n = board.size();
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            cout << (board[row][col] ? "Q " : ". ");
        }
        cout << endl;
    }
}










#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

bool IsSafe(const vector<int> &board, int row, int col, int n);
void PrintBoard(const vector<int> &board, int n);
bool SolveNQueens(int n);

int main() {
    int n;
    cout << "Enter the size of the chessboard (N): ";
    cin >> n;

    if (!SolveNQueens(n)) {
        cout << "No solution exists for N = " << n << endl;
    }
    return 0;
}

bool IsSafe(const vector<int> &board, int row, int col, int n) {
    for (int i = 0; i < row; i++) {
        if (board[i] == col) return false;
        if (abs(board[i] - col) == abs(i - row)) return false;
    }
    return true;
}

void PrintBoard(const vector<int> &board, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i] == j) {
                cout <<j<< " ";
            } else {
                cout << ". ";
            }
        }
        cout << endl;
    }
}

bool SolveNQueensUtil(int n, vector<int> &board, int row) {
    if (row == n) {
        PrintBoard(board, n);
        return true;
    }

    for (int col = 0; col < n; col++) {
        if (IsSafe(board, row, col, n)) {
            board[row] = col;

            if (SolveNQueensUtil(n, board, row + 1)) {
                return true;
            }
           
            board[row] = -1; // Backtrack
        }
    }
    return false;
}

bool SolveNQueens(int n) {
    vector<int> board(n, -1);
    if (!SolveNQueensUtil(n, board, 0)) {
        return false;
    }
    return true;
}
