#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Item {
    int value;
    int weight;
    int index;
};

void printItems(vector<Item> *vec);
int printMenu();
bool loadData(int choice, int *n, int *c, vector<Item> *items);
void solveKnapsack(int n, int c, vector<Item> *items);

int main() {
    int choice;

    int capacity = 0;  // pojemność plecaka
    int nOfItems = 0;  // liczba przedmiotów
    vector<Item> items;

    choice = printMenu();
    bool dataOK = loadData(choice, &nOfItems, &capacity, &items);

    if (dataOK) {
        if (choice == 1) {
            cout << "Pojemność plecaka: " << capacity << endl;
            printItems(&items);
        }
        solveKnapsack(nOfItems, capacity, &items);
    } else {
        cout << "Błąd";
        items.clear();
        return -1;
    }

    items.clear();
    return 0;
}

void solveKnapsack(int n, int c, vector<Item> *items) {
    vector<int> result;

    int **P = new int *[n + 1];
    for (int i = 0; i < n + 1; ++i) {
        P[i] = new int[c + 1];
        for (int j = 0; j < c + 1; ++j) {
            P[i][j] = 0;
        }
    }

    for (int i = 1; i < n + 1; ++i) {
        for (int j = 1; j < c + 1; ++j) {
            if (items->at(i - 1).weight > j) {
                P[i][j] = P[i - 1][j];
            } else {
                P[i][j] = max(P[i - 1][j], items->at(i - 1).value + P[i - 1][j - items->at(i - 1).weight]);
            }
        }
    }

    int sumOfWeights = 0;
    int x = n;
    int y = c;
    while (x > 0) {
        if (P[x - 1][y] != P[x][y]) {
            result.push_back(x);
            y -= items->at(x - 1).weight;
            sumOfWeights += items->at(x - 1).weight;
        }
        x -= 1;
    }

    cout << "Spakuj przedmioty: ";
    for (vector<int>::iterator iter = result.end() - 1; iter != result.begin() - 1; iter--) {
        cout << *iter << " ";
    }
    cout << endl;
    cout << "Łączna waga: " << sumOfWeights << endl;
    cout << "Łączna wartość: " << P[n][c] << endl;


    result.clear();
    for (int k = 0; k < n + 1; ++k) {
        delete[] P[k];
    }
    delete[] P;
}

bool loadData(int choice, int *n, int *c, vector<Item> *items) {
    if (choice == 1) {
        string address;
        cout << "Podaj adres do pliku z rozszerzeniem: ";
        cin >> address;
        int nTemp = 0; // liczba elementów
        int cTemp = 0; // pojemność plecaka

        fstream file;
        file.open(address, ios::in);
        if (file.fail()) return false;
        else {
            char tempChar = 0;
            int tempInt = 0;

            // wczytanie liczby elemntów
            file.get(tempChar);
            while (tempChar != ' ') {
                nTemp = nTemp * 10 + tempChar - 48;
                file.get(tempChar);
            }

            items->resize(nTemp);

            // wczytanie pojemności plecaka
            file.get(tempChar);
            while (tempChar != '\n') {
                cTemp = cTemp * 10 + tempChar - 48;
                file.get(tempChar);
            }


            // wczytanie kolejnych elemntów
            for (int i = 0; i < nTemp; i++) {
                file.get(tempChar);
                while (tempChar != ' ') {
                    tempInt = tempInt * 10 + tempChar - 48;
                    file.get(tempChar);
                }
                items->at(i).value = tempInt;
                tempInt = 0;
                file.get(tempChar);
                while (tempChar != '\n') {
                    tempInt = tempInt * 10 + tempChar - 48;
                    file.get(tempChar);
                }
                items->at(i).weight = tempInt;
                items->at(i).index = i + 1;
                tempInt = 0;
            }

        }
        file.close();

        *n = nTemp;
        *c = cTemp;

        return true;
    } else if (choice == 2) {
        int nTemp;
        int cTemp;

        cout << "Pojemność plecaka: ";
        cin >> cTemp;
        if (cTemp < 1) {
            cout << "pojemność plecaka nie może wynosić " << cTemp << endl;
            return false;
        }
        cout << "Liczba elementów: ";
        cin >> nTemp;
        if (nTemp < 1) {
            cout << "liczba elementów nie może wynosić " << nTemp << endl;
            return false;
        }

        items->resize(nTemp);

        cout << "Wartości i wagi kolejnych elementów: " << endl;
        for (int i = 0; i < nTemp; i++) {
            int temp = 0;
            while (temp < 1) {
                cout << i + 1 << ". wartość: ";
                cin >> temp;
                if (temp < 1) cout << "wartość musi być większa od zera" << endl;
                else items->at(i).value = temp;
            }
            temp = 0;
            while (temp < 1) {
                cout << "   waga: ";
                cin >> temp;
                if (temp < 1) cout << "waga musi być większa od zera" << endl;
                else items->at(i).weight = temp;
            }
            items->at(i).index = i + 1;
        }

        *n = nTemp;
        *c = cTemp;
        return true;
    } else return false;
}

int printMenu() {
    cout << "PROBLEM PLECAKOWY - ALGORYTM DYNAMICZNY" << endl;
    cout << "1. Wczytaj elementy z pliku" << endl;
    cout << "2. Wpisz z klawiatury" << endl;

    int choice = 0;
    while (choice != 1 && choice != 2) {
        cout << "Wybieram: ";
        cin >> choice;
    }
    cout << endl;
    return choice;
}

void printItems(vector<Item> *vec) {
    cout << "Przedmioty: " << endl;
    cout << " nr|wart|waga" << endl;
    for (vector<Item>::iterator iter = vec->begin(); iter != vec->end(); iter++) {

        cout << setw(3) << iter->index << "|" << setw(4) << iter->value << "|" << setw(4) << iter->weight << endl;
    }
}