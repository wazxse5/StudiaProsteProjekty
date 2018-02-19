#include <iostream>

using namespace std;

int loadData();
bool findScheme(int **chessboard, int size, int nbOfQueens);
void printNiceBoard(int **chessboard, int size);


int main() {
    int N = loadData(); // rozmiar szachownicy

    int **chessboard = new int *[N];
    for (int i = 0; i < N; i++) {
        chessboard[i] = new int[N];
        for (int j = 0; j < N; j++) {
            chessboard[i][j] = 0;
        }
    }

    findScheme(chessboard, N, 0);

    for (int i = 0; i < N; i++) delete[] chessboard[i];
    delete[] chessboard;

    return 0;
}

int loadData() {
    int n = 0;

    cout << "Problem N Hetmanów" << endl;
    while (n <= 0) {
        cout << "N = ";
        cin >> n;
    }

    return n;
}

bool findScheme(int **chessboard, int size, int nbOfQueens) {
    if (nbOfQueens == size) {
        printNiceBoard(chessboard, size);

        return true;
    } else {
        for (int i = 0; i < size; i++) {
            // jeżeli to miejsce jest wolne
            if (chessboard[nbOfQueens][i] == 0) {
                // tacblica zapamiętująca które pola zostały zaatakowane bezpośrednio przez ten hetman
                // jeśli jakieś pole było wcześniej atakowane to wtedy w tym kroku rekurencji nie zostanie odatakowane
                bool **changed = new bool *[size];
                for (int a = 0; a < size; a++) {
                    changed[a] = new bool[size];
                    for (int b = 0; b < size; b++) changed[a][b] = false;
                }


                // ustawienie na tym miejscu hetmana
                chessboard[nbOfQueens][i] = 1;
                // ustawienie wszystkich zaatakowanych przez niego pól (tylko w następnych wierszach)
                for (int j = nbOfQueens + 1; j < size; j++) {
                    if (chessboard[j][i] != 2) {
                        chessboard[j][i] = 2;
                        changed[j][i] = true;
                    }
                }
                for (int j = 1; j < size - nbOfQueens && j < size - i; j++) {
                    if (chessboard[nbOfQueens + j][i + j] != 2) {
                        chessboard[nbOfQueens + j][i + j] = 2;
                        changed[nbOfQueens + j][i + j] = true;
                    }
                }
                for (int j = 1; j < size - nbOfQueens && j < i; j++) {
                    if (chessboard[nbOfQueens + j][i - j] != 2) {
                        chessboard[nbOfQueens + j][i - j] = 2;
                        changed[nbOfQueens + j][i - j] = true;
                    }
                }

                // rekurencja w następnym wierszu
                bool val = findScheme(chessboard, size, nbOfQueens + 1);

                for (int j = nbOfQueens + 1; j < size; j++) {
                    if (changed[j][i]) chessboard[j][i] = 0;
                }
                for (int j = 1; j < size - nbOfQueens && j < size - i; j++) {
                    if (changed[nbOfQueens + j][i + j])chessboard[nbOfQueens + j][i + j] = 0;
                }
                for (int j = 1; j < size - nbOfQueens && j < i; j++) {
                    if (changed[nbOfQueens + j][i - j])chessboard[nbOfQueens + j][i - j] = 0;
                }
                chessboard[nbOfQueens][i] = 0;

                for (int a = 0; a < size; a++) delete[] changed[a];
                delete[] changed;

                if (val) return true;
            }
        }
        return false;
    }
}

void printNiceBoard(int **chessboard, int size) {
    cout << "\u2554";
    for (int i = 1; i < size; ++i) cout << "\u2550\u2550\u2550\u2564";
    cout << "\u2550\u2550\u2550\u2557" << endl;

    for (int i = 0; i < size; i++) {
        cout << "\u2551";
        for (int j = 0; j < size; j++) {
            if (chessboard[i][j] == 1) cout << " H ";
            else cout << "   ";
            if (j < size - 1) cout << "\u2502";
            else cout << "\u2551";

        }
        cout << endl;
        if (i < size - 1) {
            cout << "\u255F";
            for (int j = 0; j < size - 1; j++) cout << "\u2500\u2500\u2500\u253C";
            cout << "\u2500\u2500\u2500\u2562" << endl;
        } else {
            cout << "\u255A";
            for (int j = 0; j < size - 1; j++) cout << "\u2550\u2550\u2550\u2567";
            cout << "\u2550\u2550\u2550\u255D" << endl;
        }
    }
    cout << endl;
}