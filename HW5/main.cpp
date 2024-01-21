#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <random>

using namespace std;

vector<string> allLines;

int r[3][16];
double numRep = 0;
const int sizeOfR = 11;

int d[3][16];
double numDem = 0;
const int sizeOfD = 9;

int tenPercentOfSize;

int testOnEmptyLines(string line)
{
    bool isRep = false;
    int position;
    if ( line [0] == 'r') {
        isRep = true;
        position = sizeOfR;
    } else {
        position = sizeOfD;
    }
    double rChance = 0;
    double dChance = 0;
    int index = 0;

    for (int i = position; i<line.size(); i+=2)
    {
        switch (line[i])
        {
            case 'y':
                rChance += log((r[0][index] + 1) / (numRep + 2));
                dChance += log((d[0][index] + 1) / (numDem + 2));
                break;
            case 'n':
                rChance += log((r[1][index] + 1) / (numRep + 2));
                dChance += log((d[1][index] + 1) / (numDem + 2));
                break;
            case '?':
                rChance += log((r[2][index] + 1) / (numRep + 2));
                dChance += log((d[2][index] + 1) / (numDem + 2));
                break;
        }
        index++;
    }

    rChance += log((numRep + 1) / (numRep + numDem + 2));
    dChance += log((numDem + 1) / (numRep + numDem + 2));

    if ( isRep ) {
        return (rChance >= dChance);
    }
    return (dChance >= rChance);
}

void modifiDataTable(string line, int modificationNumber)
{
    bool isRep = false;
    int position;
    if ( line [0] == 'r') {
        isRep = true;
        numRep += modificationNumber;
        position = sizeOfR;
    } else {
        numDem += modificationNumber;
        position = sizeOfD;
    }
    int counter = 0;
    for (int i = position; i < line.size(); i+=2)
    {
        switch (line[i])
        {
            case 'y':
                if (isRep) {
                    r[0][counter] += modificationNumber;
                } else {
                    d[0][counter] += modificationNumber;
                }
                break;
            case 'n':
                if (isRep) {
                    r[1][counter] += modificationNumber;
                } else {
                    d[1][counter] += modificationNumber;
                }
                break;
            case '?':
                if (isRep) {
                    r[2][counter] += modificationNumber;
                } else {
                    d[2][counter] += modificationNumber;
                }
                break;
        }
        counter++;
    }
}

int main()
{
    ifstream filein("house-votes-84.data");
    string line;

    while (getline(filein, line)) {
        allLines.push_back(line);
        modifiDataTable(line, 1);
    }

    filein.close();

    tenPercentOfSize = allLines.size() / 10;
    double sumOfAllTests = 0;

    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> randomIndex(0, allLines.size() - 10);

    for (int j = 0; j < 10; j++)
    {

        int start = randomIndex(rng);
        for (int i = start; i < start + tenPercentOfSize; i++)
        {
            modifiDataTable(allLines[i], -1);
        }

        string curLine;
        double rightPredictions = 0;

        for (int i = start; i < start + tenPercentOfSize; i++)
        {
            rightPredictions += testOnEmptyLines(allLines[i]);
        }

        double testResult = rightPredictions * 100 / tenPercentOfSize;

        cout << "Result from test " << j << " : "<<testResult << "%" << endl;
        sumOfAllTests += testResult;

        for (int i = start; i < start + tenPercentOfSize; i++)
        {
            modifiDataTable(allLines[i], 1);
        }
    }

    cout <<"------------------------------------------"<< endl;
    cout <<"Result from all tests: " << sumOfAllTests / 10 << "%" <<endl;
    cout <<"------------------------------------------"<< endl;
}