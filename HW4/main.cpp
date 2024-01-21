#include <iostream>
#include <vector>
#include <random>
using namespace std;

const char emptySpace = ' ';
const char computerMove = 'O';
const char playerMove = 'X';


void endOfGame(char board[3][3], char winner){
    if( winner == 'X') {
        cout << "Player wins! \n";
    } else if ( winner == 'O') {
        cout << "Computer wins! \n";
    } else {
        cout << "Draw! \n";
    }

}

void printBoard(char board[3][3]) {
    for (short i = 0; i < 3; i++) {
        if ( i != 0 ) {
            cout << "---|---|---" << endl;
        }
        cout <<" " <<board[i][0]<< " | "<< board[i][1]<< " | "<<board[i][2]<<endl;
    }
}

char isThereWinner(char board[3][3]) {

    //Check for rows
    for (short i = 0; i < 3 ; ++i) {
        if  ( board[i][0] != emptySpace && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            return board[i][0];
        }
    }

    //Check for columns
    for (short i = 0; i < 3 ; ++i) {
        if  ( board[0][i] != emptySpace && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            return board[0][i];
        }
    }

    //Check for diagonals
    if ( board[0][0] != emptySpace && board[0][0] == board[1][1] && board[0][0] == board[2][2]) {
        return board[0][0];;
    }

    if ( board[2][0] != emptySpace && board[2][0] == board[1][1] && board[2][0] == board[0][2]) {
        return board[2][0];;
    }


    return emptySpace;
}

short evaluateMove(char board[3][3], short depth) {

    char winner = isThereWinner(board);
    if ( winner == playerMove) {
        return -10 + depth;
    } else if ( winner == computerMove) {
        return 10 - depth;
    }

    return 0;

}



bool isItDraw(char board[3][3]) {

    for (short i = 0; i < 3; i++) {
        for (short j = 0; j < 3; j++) {
            if (board[i][j] == emptySpace){
                return false;
            }
        }
    }
    return true;
}


short minimizer(char board[3][3], short alpha, short beta, short depth);


short maximizer(char board[3][3], short alpha, short beta, short depth) {

    short evaluate = evaluateMove(board, depth);
    if (evaluate != 0) {
        return evaluate;
    }

    if (isItDraw(board)) {
        return 0;
    }

    short bestMove = -11;


    for (short i = 0; i < 3; i++) {
        for (short j = 0; j < 3; j++) {
            if (board[i][j] == emptySpace) {

                board[i][j] = computerMove;

                bestMove = max(bestMove, minimizer(board, alpha, beta, depth + 1));

                board[i][j] = emptySpace;

                // if best move is lower bigger than the best explored root for minimizer
                if (bestMove >= beta) {
                    return bestMove;
                }

                alpha = max(alpha, bestMove);

            }
        }
    }

    return bestMove;
}

short minimizer(char board[3][3], short alpha, short beta, short depth) {

    short evaluation = evaluateMove(board, depth);

    if (evaluation != 0) {
        return evaluation;
    }

    if (isItDraw(board)) {
        return 0;
    }

    short bestMove = 11;

    for (short i = 0; i < 3; i++) {
        for (short j = 0; j < 3; j++) {
            if (board[i][j] == emptySpace) {

                board[i][j] = playerMove;

                bestMove = min(bestMove, maximizer(board, alpha, beta, depth + 1));

                board[i][j] = emptySpace;

                // if best move is lower than the best explored root for maximizer
                if (bestMove <= alpha) {
                    return bestMove;
                }

                beta = min(beta, bestMove);
            }
        }
    }

    return bestMove;

}

pair<short, short> minimaxAB(char board[3][3]) {

    pair<short,short> move = {-1, -1};

    short bestScore = -11;

    for (short i = 0; i < 3; i++) {
        for (short j = 0; j < 3; j++) {
            if ( board[i][j] == emptySpace){


                board[i][j] = computerMove;
                short newBoardScore = minimizer(board, -11, 11, 0);
                if(newBoardScore > bestScore) {
                    bestScore = newBoardScore;
                    move = {i, j};
                }
                board[i][j] = emptySpace;


            }

        }
    }



    return move;

}

pair<short, short> finalPosition(char board[3][3]) {
    for (short i = 0; i < 3; i++) {
        for (short j = 0; j < 3; j++) {
            if (board[i][j] == emptySpace){
                pair<short, short> finalMove = {i,j};
            }
        }
    }
}



int main() {

    char board[3][3] = {emptySpace, emptySpace, emptySpace, emptySpace, emptySpace, emptySpace, emptySpace, emptySpace, emptySpace};

    bool isPlayerOnTurn;
    cout << "Who is starting? \n" << "0 - Computer \n" << "1 - Player \n";
    cin >> isPlayerOnTurn;
    bool isComputerFirst = !isPlayerOnTurn;

    char winner = emptySpace;
    while ( winner == emptySpace) {
        if (isItDraw(board)){
            break;
        }
        if ( !isComputerFirst ) {
            printBoard(board);
        }

        if ( isPlayerOnTurn ) {
            cout<<"Player is on turn: \n";

            short x,y;
            cin >> x >> y;

            while( board[x-1][y-1] != emptySpace) {
               cout << "Wrong input. Please tray again. \n";
                cin >> x >> y;
            }
            board[x-1][y-1] = playerMove;
            isPlayerOnTurn = !isPlayerOnTurn;
        } else {
            cout<<"Computer is on turn: \n";
            if ( isComputerFirst ) {
                random_device dev;
                mt19937 rng(dev());
                uniform_int_distribution<std::mt19937::result_type> firstCorner(1, 4);

                short corner = firstCorner(rng);
                if(corner == 1){
                    board[0][0] = computerMove;
                } else if(corner == 2){
                    board[0][2] = computerMove;
                } else if(corner == 3){
                    board[2][0] = computerMove;
                } else {
                    board[2][2] = computerMove;
                }
                isComputerFirst = false;
                isPlayerOnTurn = !isPlayerOnTurn;
                continue;
            }

            pair<short, short> computerMoveXY = minimaxAB(board);

            board[computerMoveXY.first][computerMoveXY.second] = computerMove;

            isPlayerOnTurn = !isPlayerOnTurn;
        }

        winner = isThereWinner(board);
    }

    printBoard(board);
    endOfGame(board, winner);

}

// samo vreme bez pamet se optimizira1
