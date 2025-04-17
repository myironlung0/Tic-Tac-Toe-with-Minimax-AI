#include <iostream>
#include <ctime>
#include <cstdlib>

void getPlayerMove(char board[3][3], char player);
void getComputerMove(char board[3][3], char computer, char player);
int evaluate(char board[3][3], char computer, char player);
int minimax(char board[3][3], int depth, bool isMaximizing, char computer, char player);
bool checkWin(char board[3][3], char character);
bool checkTie(char board[3][3]);

void getPlayerMove(char board[3][3], char player, int row, int col, bool* playerTurn){
    if(row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' '){
        board[row][col] = player;
        *playerTurn = false;
    } else {
        std::cout << "That spot is already taken or out of bounds. \n";
        *playerTurn = true;
    }
}

void getComputerMove(char board[3][3], char computer, char player){
   int bestScore = -1000;
   int bestMoveCol = -1;
   int bestMoveRow = -1;

   for (int i = 0; i < 3; i++){
    for (int j = 0; j < 3; j++){
        if (board[i][j] == ' ') {
            board[i][j] = computer;
            int moveScore = minimax(board, 0, false, computer, player);
            board[i][j] = ' ';

            if (moveScore > bestScore){
                bestScore = moveScore;
                bestMoveRow = i;
                bestMoveCol = j;
            }
        }     
    }
   }
   board[bestMoveRow][bestMoveCol] = computer;

}

//evaluates the board state
int evaluate(char board[3][3], char computer, char player){
    if(checkWin(board, computer)) return 10;
    if(checkWin(board, player)) return -10;
    
    return 0; //game ongoing or tie
}

/*
minimax algorithm with depth-limited search,
recursively evaluates all possible moves to find the optimal move for the AI
*/

int minimax(char board[3][3], int depth, bool isMaximizing, char computer, char player){
    //base case
    int score = evaluate(board, computer, player);

    if(score == 10){
        score = score - depth;
    }

    if(score == -10){
        score = score + depth;
    }

    if(score != 0) return score;

   //check for a tie (full board)
   if (checkTie(board)) return 0;

   //maximizing player (computer)
    if(isMaximizing){
        int bestScore = -1000;
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                if (board[i][j] == ' '){
                    board[i][j] = computer; //simulate the move
                    bestScore = std::max(bestScore, minimax(board, depth+1, false, computer, player));
                    board[i][j] = ' '; //backtrack
                }
            }
        }
        return bestScore;
    
    //minimizing player (human)
    }else{
        int bestScore = 1000;
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                if (board[i][j] == ' '){
                    board[i][j] = player; //simulate player's move
                    bestScore = std::min(bestScore, minimax(board, depth+1, true, computer, player));
                    board[i][j] = ' '; //backtrack
                }
            }
        }
        return bestScore;
    }


}

//checks if anybody has won
bool checkWin(char board[3][3], char character){
    //rows
    for(int i=0; i<3; i++){
        if(board[i][0] == character && board[i][1] == character && board[i][2] == character){
            return true;
        }
    }

    //cols
    for(int i=0; i<3; i++){
        if(board[0][i] == character && board[1][i] == character && board[2][i] == character){
            return true;
        }
    }

    //diagonals
    if (board[0][0] == character && board[1][1] == character && board[2][2] == character) {
        return true;
    }
    if (board[0][2] == character && board[1][1] == character && board[2][0] == character) {
        return true;
    }
    return false;

}

//checks for a tie
bool checkTie(char board[3][3]){

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                return false;
            }
        }
    }
    return true;
    
}