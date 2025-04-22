#include <bits/stdc++.h>

using namespace std;

// Define the board size
const int BOARD_SIZE = 3;

struct GameState
{
    vector < vector < char >> board;
    int depth;
    int heuristic;
    bool isMaximizing;

    GameState(vector < vector < char >> b, int d, int h, bool maxPlayer): board(b), depth(d), heuristic(h), isMaximizing(maxPlayer) {}

    bool operator < (const GameState & other) const
    {
        return heuristic > other.heuristic;
        // Lower heuristic has higher priority
    }
};

// Function to check if the game is over
int evaluate(const vector < vector < char >> & board)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2])
        {
            if (board[i][0] == 'X')
                return +10;
            if (board[i][0] == 'O')
                return -10;
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i])
        {
            if (board[0][i] == 'X')
                return +10;
            if (board[0][i] == 'O')
                return -10;
        }
    }

    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
    {
        if (board[0][0] == 'X')
            return +10;
        if (board[0][0] == 'O')
            return -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0])
    {
        if (board[0][2] == 'X')
            return +10;
        if (board[0][2] == 'O')
            return -10;
    }

    return 0; // No winner
}

// Function to calculate heuristic value
int heuristic(const vector < vector < char >> & board, bool isMaximizing)
{
    int score = evaluate(board);
    if (score == 10 || score == -10)
        return score; // Return the winning score
    return 0; // Neutral heuristic for non-terminal states
}

bool isBoardFull(const vector < vector < char >> & board)
{
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            if (board[i][j] == '_')
                return false;
    return true;
}

// Function to generate all possible next moves
vector < GameState > generateNextStates(const GameState & state)
{
    vector < GameState > nextStates;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (state.board[i][j] == '_')
            {
                // Create a new state with the move applied
                vector < vector < char >> newBoard = state.board;
                newBoard[i][j] = state.isMaximizing ? 'X' : 'O';
                int h = heuristic(newBoard, !state.isMaximizing);
                nextStates.emplace_back(newBoard, state.depth + 1, h, !state.isMaximizing);
            }
        }
    }
    return nextStates;
}

// A* algorithm to find the best move
pair < int, int > findBestMove(vector < vector < char >> & board, bool isMaximizing)
{
    priority_queue < GameState > pq;
    pq.emplace(board, 0, heuristic(board, isMaximizing), isMaximizing);

    GameState bestState = pq.top();

    while (!pq.empty())
    {
        GameState currentState = pq.top();
        pq.pop();

        // Check if the game is over
        if (evaluate(currentState.board) != 0 || isBoardFull(currentState.board))
        {
            bestState = currentState;
            break;
        }

        // Generate next states and add them to the priority queue
        vector < GameState > nextStates = generateNextStates(currentState);
        for (const auto & nextState: nextStates)
        {
            pq.push(nextState);
        }
    }

    // Find the move that led to the best state
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] != bestState.board[i][j])
            {
                return {
                    i,
                    j
                };
            }
        }
    }
    return {-1,-1};
}

void printBoard(const vector < vector < char >> & board)
{
    for (const auto & row: board)
    {
        for (char cell: row)
        {
            cout << cell << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int main()
{
    vector < vector < char >> board = {
        {
            '_',
            '_',
            '_'
        },
        {
            '_',
            '_',
            '_'
        },
        {
            '_',
            '_',
            '_'
        }
    };

    cout << "Welcome to Tic-Tac-Toe!\n";
    cout << "You are 'O', and the AI is 'X'.\n";

    bool isPlayerTurn = true;

    while (true)
    {
        printBoard(board);

        // Check for game end
        int score = evaluate(board);
        if (score == 10)
        {
            cout << "AI wins!\n";
            break;
        }
        else if (score == -10)
        {
            cout << "You win!\n";
            break;
        }
        else if (isBoardFull(board))
        {
            cout << "It's a draw!\n";
            break;
        }

        if (isPlayerTurn)
        {
            cout << "Your turn. Enter row and column (0-indexed): ";
            int row, col;
            cin >> row >> col;
            if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board[row][col] == '_')
            {
                board[row][col] = 'O';
                isPlayerTurn = false;
            }
            else
            {
                cout << "Invalid move. Try again.\n";
            }
        }
        else
        {
            cout << "AI is thinking...\n";
            pair < int, int > bestMove = findBestMove(board, true);
            board[bestMove.first][bestMove.second] = 'X';
            isPlayerTurn = true;
        }
    }

    return 0;
}
