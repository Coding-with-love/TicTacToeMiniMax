#include <iostream>  // For input/output operations
#include <vector>    // For using the vector data structure
#include <limits>    // For numeric limits (used in minimax)

using namespace std;

// Function to print the current state of the Tic-Tac-Toe board
void printBoard(const vector<char>& board) {
    cout << endl;
    for (int i = 0; i < 3; ++i) {  // Iterate over 3 rows
        cout << " ";
        for (int j = 0; j < 3; ++j) {  // Iterate over 3 columns in each row
            cout << board[i * 3 + j];  // Access the correct board position
            if (j < 2) cout << " | ";  // Print vertical dividers between columns
        }
        cout << endl;
        if (i < 2) cout << "---+---+---" << endl;  // Print horizontal dividers between rows
    }
    cout << endl;
}

// Function to check if a player has won the game
bool checkWin(const vector<char>& board, char player) {
    // Win conditions: 8 possible ways to win (3 rows, 3 columns, 2 diagonals)
    const int win_conditions[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
        {0, 4, 8}, {2, 4, 6}             // Diagonals
    };

    // Check each win condition to see if the player occupies all 3 positions
    for (const auto& condition : win_conditions) {
        if (board[condition[0]] == player && 
            board[condition[1]] == player && 
            board[condition[2]] == player) {
            return true;  // Player has won
        }
    }
    return false;  // No win found
}

// Function to check if the board is full (i.e., no empty spaces left)
bool isBoardFull(const vector<char>& board) {
    // Iterate through the board and check if any cell is still empty (' ')
    for (char c : board) {
        if (c == ' ') {
            return false;  // Found an empty space, board is not full
        }
    }
    return true;  // No empty spaces, board is full
}

// Minimax algorithm: AI decision-making function
int minimax(vector<char>& board, int depth, bool isMaximizing, char aiPlayer, char humanPlayer) {
    // Check if the AI has won; return a positive score
    if (checkWin(board, aiPlayer)) {
        return 10 - depth;  // The earlier the win, the better (higher score)
    }

    // Check if the human has won; return a negative score
    if (checkWin(board, humanPlayer)) {
        return depth - 10;  // The later the human wins, the worse for AI (lower score)
    }

    // Check if the board is full and no one has won (it's a tie)
    if (isBoardFull(board)) {
        return 0;  // A tie results in a score of 0
    }

    // Maximizing step for the AI
    if (isMaximizing) {
        int bestScore = numeric_limits<int>::min();  // Start with the lowest possible score
        for (int i = 0; i < 9; ++i) {  // Iterate over all board positions
            if (board[i] == ' ') {  // If the position is empty
                board[i] = aiPlayer;  // AI makes a move
                int score = minimax(board, depth + 1, false, aiPlayer, humanPlayer);  // Recurse for the next step
                board[i] = ' ';  // Undo the move (backtracking)
                bestScore = max(bestScore, score);  // Choose the move with the highest score
            }
        }
        return bestScore;
    } 
    // Minimizing step for the human player
    else {
        int bestScore = numeric_limits<int>::max();  // Start with the highest possible score
        for (int i = 0; i < 9; ++i) {  // Iterate over all board positions
            if (board[i] == ' ') {  // If the position is empty
                board[i] = humanPlayer;  // Human makes a move
                int score = minimax(board, depth + 1, true, aiPlayer, humanPlayer);  // Recurse for the next step
                board[i] = ' ';  // Undo the move (backtracking)
                bestScore = min(bestScore, score);  // Choose the move with the lowest score
            }
        }
        return bestScore;
    }
}

// Function to find the best possible move for the AI
int bestMove(vector<char>& board, char aiPlayer, char humanPlayer) {
    int bestScore = numeric_limits<int>::min();  // Start with the lowest score possible
    int move = -1;  // Variable to store the best move position

    for (int i = 0; i < 9; ++i) {  // Iterate over all board positions
        if (board[i] == ' ') {  // If the position is empty
            board[i] = aiPlayer;  // AI makes a move
            int score = minimax(board, 0, false, aiPlayer, humanPlayer);  // Evaluate the move with minimax
            board[i] = ' ';  // Undo the move
            if (score > bestScore) {  // If this move is better than the previous best
                bestScore = score;  // Update the best score
                move = i;  // Store the position of this best move
            }
        }
    }
    return move;  // Return the index of the best move
}

// Main function where the game is played
int main() {
    vector<char> board(9, ' ');  // Initialize an empty board with 9 positions
    char humanPlayer, aiPlayer;  // Variables to store the player's and AI's markers
    string input;  // To get user input

    // Let the user choose their marker (X or O)
    do {
        cout << "Choose your marker (X/O): ";  // Prompt for marker selection
        getline(cin, input);  // Read input
        if (!input.empty())  // Check if input is not empty
            humanPlayer = toupper(input[0]);  // Set the player's marker, convert to uppercase
    } while (humanPlayer != 'X' && humanPlayer != 'O');  // Ensure valid choice (X or O)

    // Assign the AI player the opposite marker
    aiPlayer = (humanPlayer == 'X') ? 'O' : 'X';

    // X always goes first, so set currentPlayer to X initially
    char currentPlayer = 'X';

    printBoard(board);  // Print the empty board initially

    // Main game loop
    while (true) {
        // Human player's turn
        if (currentPlayer == humanPlayer) {
            int move = -1;
            while (true) {
                cout << "Enter your move (1-9): ";  // Prompt the user to make a move
                getline(cin, input);  // Read input
                if (!input.empty()) {
                    move = input[0] - '1';  // Convert the input to a board index (1-9 maps to 0-8)
                    // Ensure the move is within bounds and the cell is empty
                    if (move >= 0 && move < 9 && board[move] == ' ')
                        break;
                    else
                        cout << "Invalid move. Try again." << endl;  // Invalid move
                }
            }
            board[move] = humanPlayer;  // Update the board with the human's move
        } 
        // AI's turn
        else {
            int move = bestMove(board, aiPlayer, humanPlayer);  // Find the best move for AI
            board[move] = aiPlayer;  // Update the board with AI's move
            cout << "Computer places '" << aiPlayer << "' in position " << move + 1 << "." << endl;
        }

        printBoard(board);  // Print the board after each move

        // Check if the current player has won
        if (checkWin(board, currentPlayer)) {
            if (currentPlayer == humanPlayer)
                cout << "Congratulations! You win!" << endl;
            else
                cout << "Computer wins! Better luck next time." << endl;
            break;  // Exit the game loop if someone wins
        }

        // Check if the board is full and it's a tie
        if (isBoardFull(board)) {
            cout << "It's a tie!" << endl;
            break;  // Exit the game loop if it's a tie
        }

        // Switch turns: alternate between 'X' and 'O'
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    return 0;  // End of the program
}
