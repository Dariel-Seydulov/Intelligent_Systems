#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <algorithm>
#include <thread>

using namespace std;

short input;

bool isBoardSolvable(vector<vector<short>> matrix, short size, short positionOfNullX) {
    vector<int> arr;
    for (short i = 0; i < size; i++)
    {
        for (short j = 0; j < size; j++)
        {
            arr.push_back(matrix[i][j]);
        }
    }
    short invCount = 0;
    for (short i = 0; i < arr.size(); i++)
    {
        for (short j = i + 1; j < arr.size(); j++)
        {
            if (arr[i] > arr[j] && arr[i] != 0 && arr[j] != 0)
            {
                invCount++;
            }
        }
    }

    if (size % 2 == 1)
    {
            return !((invCount) % 2);
    }
    else
    {
        if(positionOfNullX % 2 == 0) {
            return ((invCount) % 2);
        }
        else {
            return !(invCount%2);
        }

    }


}
int manhattanForOne(short xNow, short yNow, short xTo, short yTo) {
    int ans = abs(xNow-xTo) + abs(yNow- yTo);

    return ans;
}

int manhattanDistance(vector<vector<short>> matrix, short size) {

    int ans = 0;
    short null;
    if(input == -1) {
        null = size*size-1;
        input = size*size-1;
    } else {
        null = input;
    }
    for( short i = 0; i < size; i ++) {
        for( short j = 0; j < size; j ++) {
            if ( matrix[i][j] == 0) {
                continue;
            }
            short number = matrix[i][j] ;
           if ( number > null ) {
               number++;
           }
           number--;
           short x = number / size;
           short y = number % size;

           ans = ans + (abs(i - x) + abs(j - y));
        }
    }
    return ans;
}

struct Move {
    vector<vector<short>> board;
    short heursitic;
    short manhattan;
    short xOfNull;
    short yOfNull;
    short moveCount;
    pair<short,short> lastMove;
    string currMoves;

    Move(vector<vector<short>> board, short heuristic, short manhattan, short xOfNull, short yOfNull,short moveCount, string currMoves, pair<short,short> lastMove) {
        this->board = board;
        this->heursitic = heuristic;
        this->manhattan = manhattan;
        this->xOfNull = xOfNull;
        this->yOfNull = yOfNull;
        this->moveCount = moveCount;
        this->currMoves = currMoves;
        this->lastMove = lastMove;
    }

};

class CompareMoves {
public:
    bool operator() (Move a, Move b) {
        return a.heursitic > b.heursitic;
    }
};

string stringMove(pair<short,short> move) {

    if (move.first == 0 && move.second == 1){
        return "right\n";
    }
    if (move.first == 0 && move.second == -1){
        return "left\n";
    }
    if (move.first == 1 && move.second == 0){
        return "down\n";
    }
    if (move.first == -1 && move.second == 0){
        return "up\n";
    }

}


int main() {

    short n;
    cin >> n;
    short positionOfNull;
    cin >> positionOfNull;
    input=positionOfNull;

    short size = sqrt(n+1);

    vector<vector<short>> finalmatrix;

    short number=1;
    short xOfNull=0;
    short yOfNull=0;

    for( short i = 0; i < size; i ++) {
        vector<short> row;
        for( short j = 0; j < size; j ++) {
            if( positionOfNull == -1 && i == size - 1 && j == 0 ) {

                row.push_back(0);
                continue;
            }
            if(size*i + j == positionOfNull) {

                row.push_back(0);
                continue;
            }
            row.push_back(number);
            number++;
        }
        finalmatrix.push_back(row);
    }

    vector<vector<short>> startmatrix (size, vector<short>(size));

    short zeroRow;

    for (short i = 0; i < size; i++) {
        for (short j = 0; j < size; j++) {
            cin >> startmatrix[i][j];
            if (startmatrix[i][j] == 0) {
                xOfNull=i;
                yOfNull=j;
                zeroRow = size - i;
            }
        }
    }
    if(!isBoardSolvable(startmatrix, size, zeroRow)) {
        cout << -1 ;
        return 0;
    }

    priority_queue<Move, vector<Move>, CompareMoves> moves;
    int m = manhattanDistance(startmatrix, size);
    int h = m;
    Move zeroMove (startmatrix, h, m, xOfNull, yOfNull, 0, "", {0, 0});
    moves.push(zeroMove);
    int moveCount=0;
    auto start_time = std::chrono::high_resolution_clock::now();
    while(true){
        Move currentMove = moves.top();

        if (currentMove.manhattan == 0 ) {
            auto stop_time = std::chrono::high_resolution_clock::now();
            auto duration =  std::chrono::duration_cast<std::chrono::seconds >(stop_time - start_time);
            cout << duration.count() << endl;
            cout << currentMove.moveCount << endl;
            cout << currentMove.currMoves;
            break;
        }
        moves.pop();
        moveCount = currentMove.moveCount + 1;

        pair<short, short> left = {0,-1};
        pair<short, short> right = {0,1};
        pair<short, short> up = {-1,0};
        pair<short, short> down = {1,0};
        vector<pair<short,short>> oneMoves= {left,right,up,down};

        for(pair<short,short> oneMove: oneMoves) {
            vector<vector<short>> currentMatrix = currentMove.board;
            short currentX = currentMove.xOfNull;
            short currentY = currentMove.yOfNull;
            if ((currentX + oneMove.first < size) && (currentY + oneMove.second < size) && (currentX + oneMove.first >= 0) && (currentY + oneMove.second >= 0)) {
                if(currentMove.lastMove.first * oneMove.first + currentMove.lastMove.second * oneMove.second < 0 ) {
                    continue;
                }
                short numberNow = currentMatrix[currentX+oneMove.first][currentY+oneMove.second];
                if ( numberNow > input ) {
                    numberNow++;
                }
                numberNow--;
                short x = numberNow / size;
                short y = numberNow % size;
                int m = (currentMove.manhattan +
                        manhattanForOne(currentX,currentY,x,y) -
                        manhattanForOne(currentX+oneMove.first,currentY+oneMove.second,x,y) );
                int h = m + moveCount;
                swap(currentMatrix[currentX][currentY], currentMatrix[currentX+oneMove.first][currentY+oneMove.second]);
                Move move1 (currentMatrix,
                               h,
                               m,
                               currentX+oneMove.first,
                               currentY+oneMove.second,
                               moveCount,
                               currentMove.currMoves+stringMove(oneMove),
                               oneMove);
                moves.push(move1);
            }
        }
    }
    return 0;
}



// evristikta - konstistentna (neravenstvo na triagalnik) - optiamlna
// - da dava pomalko ili ravno na ochakvanoto
// - pravilo na trigulnik
// koe e f h g

