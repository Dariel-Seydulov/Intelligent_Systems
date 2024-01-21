#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

int n;
int* queensPositions;
int* rowQueensCount;
int* primaryDiagonal;  // \ //
int* secondaryDiagonal; // / //

void staringPositions() {
    int column = 0;
    if (n % 6 == 2)
    {
        for (int i = 0; i < n / 2; i++)
        {
            queensPositions[i] = column;
            column += 2;
        }
        column = 1;
        for (int i = n / 2; i < n; i++)
        {
            queensPositions[i] = column;
            column += 2;
        }
    }
    else if (n % 6 == 3)
    {
        column = 1;
        for (int i = 0; i < n / 2; i++)
        {
            queensPositions[i] = column;
            column += 2;
        }
        column = 2;
        for (int i = n / 2; i < n - 1; i++)
        {
            queensPositions[i] = column;
            column += 2;
        }
        queensPositions[n - 1] = 0;
    }
    else
    {
        for (int i = 1; i < n; i += 2)
        {
            queensPositions[i] = column;
            column++;
        }

        for (int i = 0; i < n; i += 2)
        {
            queensPositions[i] = column;
            column++;
        }
    }


//    for (int i = 0; i < n; i++) {
//
//        queensPositions[column]=i;
//        rowQueensCount[i]++;
//        secondaryDiagonal[i + column]++;
//        primaryDiagonal[n - 1 - i + column]++;
//
//        column += 2;
//        if (column >= n ) {
//            column = 0;
//        }
//
//    }
}

void printBoard() {

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if(queensPositions[j]==i){
                cout<<'*'<<' ';
            } else {
                cout<<"- ";
            }
        }
        cout<< endl;
    }
}

int columnWithMaxConflicts() {
    int maxConflict = 1;
    vector<int> columnsWithMaxConflict;

    int currentQueenRow,currentConflict;
    for (int currentCol = 0; currentCol < n; currentCol++) {

        currentQueenRow = queensPositions[currentCol];
        currentConflict =
                rowQueensCount[currentQueenRow]
                + primaryDiagonal[n - 1 + currentCol - currentQueenRow]
                + secondaryDiagonal[currentCol + currentQueenRow]
                -3;

        if ( currentConflict == maxConflict ) {
            columnsWithMaxConflict.push_back(currentCol);
        } else if ( currentConflict > maxConflict ) {
            maxConflict = currentConflict;
            columnsWithMaxConflict.clear();
            columnsWithMaxConflict.push_back(currentCol);
        }
    }

    if ( columnsWithMaxConflict.empty()) {
        return -1;
    }

    int randomIndex = rand() % columnsWithMaxConflict.size();

    return columnsWithMaxConflict[randomIndex];
}

int rowWithMinConflicts(int column) {
    int minConflicts = n;
    vector<int> rowOfMinConflicts;

    for (int currentRow= 0; currentRow < n; currentRow++) {

        int temp=0;
        if( queensPositions[column] == currentRow ) {
            temp = 3;
        }

        int currentConflicts = rowQueensCount[currentRow]
                               + primaryDiagonal[n - 1 + column - currentRow]
                               + secondaryDiagonal[column + currentRow]
                               - temp;

        if( currentConflicts == minConflicts ) {
            rowOfMinConflicts.push_back(currentRow);
        } else if ( currentConflicts < minConflicts ) {
            minConflicts = currentConflicts;
            rowOfMinConflicts.clear();
            rowOfMinConflicts.push_back(currentRow);
        }
    }

    int randomIndex = rand() % rowOfMinConflicts.size();

    return rowOfMinConflicts[randomIndex];
}

void solveBoard() {

    bool again = true;
    for (int i = 0; i < n; i++){
        int column = columnWithMaxConflicts();
        if (column == -1 ){
            again = false;
            break;
        }
        int nextRow = rowWithMinConflicts(column);

        int rowNow = queensPositions[column];
        rowQueensCount[rowNow]--;
        primaryDiagonal[n - 1 + column - rowNow]--;
        secondaryDiagonal[column + rowNow]--;

        queensPositions[column] = nextRow;
        rowQueensCount[nextRow]++;
        primaryDiagonal[n - 1 + column - nextRow]++;
        secondaryDiagonal[column + nextRow]++;

    }

    if (again) {
        solveBoard();
    }

}

int main() {

    cin >> n;

    if(n < 4) {
        cout << -1;
        return 0;
    }

    queensPositions = new int[n];
    rowQueensCount = new int[n] { 0 };
    secondaryDiagonal = new int[2 * n - 1]{ 0 };
    primaryDiagonal = new int[2 * n - 1]{ 0 };

    staringPositions();
    auto start_time = std::chrono::high_resolution_clock::now();
    solveBoard();
    auto stop_time = std::chrono::high_resolution_clock::now();

    auto duration =std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    cout << "Duration: " << duration.count() << '\n';

    //printBoard();


    return 0;
}

// optimizirai inicializaciqta  0 1 4 5 kato sega 2 nadolu 3 nagore nadqsn
// Vajno :
// деф задачата в началото множество променлаиви с множество допустими стойности, търсим да удовлетворим някаво множесто от променливи.
// унарни бинарни и от по висока степен ограничения.
