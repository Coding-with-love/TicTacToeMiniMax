#include <iostream>
#include <vector>
#include <limits>

using namespace std;
//prints the board
void printBoard(const vector<char>& board){
    cout << endl;
    for (int i = 0; i < 3; ++i){
        cout << " ";
        for (int j = 0; j < 3; ++j){
            cout << board[i * 3 * j];
            if (j < 2) cout << " | ";
        }
        cout << endl;
        if (i < 2) cout << "---+---+---" << endl;
    }
    cout << endl;
}

//checks if a player has won
bool checkWin (const vector<char>& board, char player){
    const int win_conditions[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8}, //Rows
        {0,3,6}, {1,4,7}, {2,5,8}, //Columns
        {0,4,8}, {2,4,6} //diagonals
    };
    for(const auto& condition : win_conditions){
        if(board[condition[0]] == player && board[condition[1]] == player && board[condition[2]] == player){
            return true;
        }
    }
    return false;
}

//Checks if the board is full
bool isBoardFull (const vector<char>& board){
    for (char c : board){
        if (c == ' '){
            return false;
        } 
    }
    return true;
}

//Minimax algorithm 
int minimax(vector<char>& board, int depth, bool isMaximizing, char aiPlayer, char humanPlayer){
    if (checkWin(board,aiPlayer)){
        return 10 - depth;
    }
    if (checkWin(board, humanPlayer)){
        return depth - 10;
    }
    if (isBoardFull(board)){
        return 0;
    }

     if (isMaximizing) {
        int bestScore = numeric_limits<int>::min();
        for (int i = 0; i < 9; ++i) {
            if (board[i] == ' ') {
                board[i] = aiPlayer;
                int score = minimax(board, depth + 1, false, aiPlayer, humanPlayer);
                board[i] = ' ';
                bestScore = max(bestScore, score);
            }
        }
        return bestScore;
    } else {
        int bestScore = numeric_limits<int>::max();
        for (int i = 0; i < 9; ++i) {
            if (board[i] == ' ') {
                board[i] = humanPlayer;
                int score = minimax(board, depth + 1, true, aiPlayer, humanPlayer);
                board[i] = ' ';
                bestScore = min(bestScore, score);
            }
        }
        return bestScore;
    }
}

// finding the best move for the AI
int bestMove(vector<char>& board, char aiPlayer, char humanPlayer) {
    int bestScore = numeric_limits<int>::min();
    int move = -1;
    for (int i = 0; i < 9; ++i) {
        if (board[i] == ' ') {
            board[i] = aiPlayer;
            int score = minimax(board, 0, false, aiPlayer, humanPlayer);
            board[i] = ' ';
            if (score > bestScore) {
                bestScore = score;
                move = i;
            }
        }
    }
    return move;
}

int main() {
    vector<char> board(9, ' ');
    char humanPlayer, aiPlayer;
    string input;

    // Let the user choose their marker
    do {
        cout << "Choose your marker (X/O): ";
        getline(cin, input);
        if (!input.empty())
            humanPlayer = toupper(input[0]);
    } while (humanPlayer != 'X' && humanPlayer != 'O');

    aiPlayer = (humanPlayer == 'X') ? 'O' : 'X';

    char currentPlayer = 'X';

    printBoard(board);

    while (true) {
        if (currentPlayer == humanPlayer) {
            int move = -1;
            while (true) {
                cout << "Enter your move (1-9): ";
                getline(cin, input);
                if (!input.empty()) {
                    move = input[0] - '1';
                    if (move >= 0 && move < 9 && board[move] == ' ')
                        break;
                    else
                        cout << "Invalid move. Try again." << endl;
                }
            }
            board[move] = humanPlayer;
        } else {
            int move = bestMove(board, aiPlayer, humanPlayer);
            board[move] = aiPlayer;
            cout << "Computer places '" << aiPlayer << "' in position " << move + 1 << "." << endl;
        }

        printBoard(board);

        if (checkWin(board, currentPlayer)) {
            if (currentPlayer == humanPlayer)
                cout << "Congratulations! You win!" << endl;
            else
                cout << "Computer wins! Better luck next time." << endl;
            break;
        }
        if (isBoardFull(board)) {
            cout << "It's a tie!" << endl;
            break;
        }

        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    return 0;
}