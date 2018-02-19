#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <ctime>

using namespace std;

int printMenu(int *nbOfVariables, string *path);
void printFunction(vector<vector<int>> &function, vector<bool> &values, string typeOfPrinting);
void printTime(chrono::time_point<chrono::system_clock> start, chrono::time_point<chrono::system_clock> end);

bool readFunctionFromFile(vector<vector<int>> &function, vector<bool> &values, string path, int *nbOfVariables);
void generateFunction(vector<vector<int>> &function, vector<bool> &values, int nbOfVariables);
bool calculateFunction(vector<vector<int>> &function, vector<bool> &values);
void generateNewValues(vector<bool> &values);

bool WalkSat(vector<vector<int>> &function, vector<bool> values);
bool GSat(vector<vector<int>> &function, vector<bool> values);

const int MAX_TRIES1 = 100; // maksymalna liczba prób dla zadanego zestawu wartości zmiennych
const int MAX_TRIES2 = 20; // maksymalna liczba nowych losowań zestawu zmiennych

int main() {
    srand((unsigned int) time(NULL));
    chrono::time_point<chrono::system_clock> start, end; // zmienne do mierzenia czasu wykonania

    vector<vector<int>> function; // dwuwymiarowy wektor opisujący strukturę (wzór) funkcji
    vector<bool> values; // wektor pamiętajcy wartości zmiennych
    int nbOfVariables; // liczba zmiennych
    int choice; // wybór opcji z menu
    string path; // ścieżka do pliku z danymi

    // wyświetlenie menu, wczytanie liczby zmiennych
    choice = printMenu(&nbOfVariables, &path);
    switch(choice) {
        case 1: // wczytanie funkcji
            if(!readFunctionFromFile(function, values, path, &nbOfVariables)) return -1; break;
        case 2: // wygenerowanie funkcji
            generateFunction(function, values, nbOfVariables); break;
    }


    // wyświetlenie wzoru funkcji
    printFunction(function, values, "formula");
    cout << endl << endl;


    cout << "WalkSAT: " ; // algorytm WalkSAT
    start = chrono::system_clock::now();
    bool good = WalkSat(function, values);
    end = chrono::system_clock::now();
    if(good) printTime(start, end);
    else cout << "NIE" << endl;

    cout << "GSAT: "; // algorytm GSAT
    start = chrono::system_clock::now();
    good = GSat(function, values);
    end = chrono::system_clock::now();
    if(good) printTime(start, end);
    else cout << "NIE" << endl;


    values.clear();
    function.clear();
    return 0;
}

int printMenu(int *nbOfVariables, string *path) {
    int temp = 0; // ilość zmiennych do wczytania
    int choice = 0; // wybór z menu

    cout << "Problem spełnialności, porównanie algorytmów WalkSat i GSat" << endl;
    cout << "1. Wczytaj funkcję z pliku " << endl;
    cout << "2. Wygeneruj funkcję" << endl;
    while (choice != 1 && choice != 2) {
        cout << "Wybieram: ";
        cin >> choice;
    }

    if(choice == 1) {
        cout << "Podaj ścieżkę do pliku z rozszerzeniem: ";
        cin >> *path;
    }

    if(choice == 2) {
        // dopóki nie zostanie podana prawidłowa liczba
        while (temp < 1) {
            cout << "Podaj liczbę zmiennych: ";
            cin >> temp;
        }
        *nbOfVariables = temp;
    }

    return choice;
}

void printFunction(vector<vector<int>> &function, vector<bool> &values, string typeOfPrinting) {
    /* funkcja printFunction umożliwia wyświetlanie funkcji w trzech trybach
     * formula - wyświetla wzór funkcji
     * values - wyświetla funkcję ze wstawionymi aktualnymi wartościami zmiennych
     * definition - tylko nazwa i wartości zmiennych
     * */

    // jeżeli wprowadzono błędny typ wyświetlania
    if (typeOfPrinting != "values" && typeOfPrinting != "formula" && typeOfPrinting != "definition") {
        cout << "error in printing function, wrong argument typeOfPrinting";
        return;
    }

    unsigned long nbOfVariables = values.size();
    unsigned long nbOfClauses = function.size();

    // wyświetlenie nazwy funkcji i zmiennych
    cout << "f(";
    for (int i = 0; i < nbOfVariables; ++i) {
        if (typeOfPrinting == "formula") cout << static_cast<char>(65 + i);
        else if (typeOfPrinting == "values" || typeOfPrinting == "definition") cout << values.at(i);

        if (i + 1 < nbOfVariables) cout << ",";
    }
    cout << ")";

    if (typeOfPrinting == "definition") return;

    cout << " = ";

    // wyświetlenie wzoru funkcji
    for (int i = 0; i < nbOfClauses; ++i) {
        bool empty = true; // sprawdzenie czy nawias zawiera zmienne
        for (int k = 0; k < nbOfVariables; ++k) {
            if (function.at(i).at(k) == 0 || function.at(i).at(k) == 1) empty = false;
        }
        empty = false;

        // jeżeli nawias nie jest pusty
        if (!empty) {
            cout << "(";
            for (int j = 0; j < nbOfVariables; ++j) {
                bool nextExist = false;
                for (int k = j + 1; k < nbOfVariables; ++k) {
                    if (function.at(i).at(k) == 0 || function.at(i).at(k) == 1) nextExist = true;
                }
                if (typeOfPrinting == "formula") {
                    if (function.at(i).at(j) == 0) cout << " " << static_cast<char>(65 + j);
                    else if (function.at(i).at(j) == 1) cout << "-" << static_cast<char>(65 + j);
                } else if (typeOfPrinting == "values") {
                    if (function.at(i).at(j) == 0) cout << " " << values.at(j);
                    else if (function.at(i).at(j) == 1) {
                        cout << " " << !(values.at(j) == true);
                    }
                }
                if (function.at(i).at(j) < 2 && j + 1 < nbOfVariables && nextExist) cout << "|";
            }
            cout << ")";
            if (i + 1 < nbOfClauses) cout << " & ";
        }
    }
}

void printTime(chrono::time_point<chrono::system_clock> start, chrono::time_point<chrono::system_clock> end) {
    double time_elapsed = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout << endl << "   czas: ";
    if(time_elapsed < 1000) cout << time_elapsed << " mikrosekund";
    else {
        time_elapsed /= 1000;
        if(time_elapsed < 1000) cout << time_elapsed << " milisekund";
        else {
            time_elapsed /= 1000;
            cout << time_elapsed << " sekund";
        }
    }
    cout << endl;
}


bool readFunctionFromFile(vector<vector<int>> &function, vector<bool> &values, string path, int *nbOfVariables) {
    fstream file;
    file.open(path, ios::in);

    char tempChar = 0;
    if (file.good()) {
        while (!file.eof()) {
            vector<int> tempVec;
            file.get(tempChar);
            while (tempChar != '\n') {
                tempVec.push_back(tempChar - 48);
                file.get(tempChar);
            }
            function.push_back(tempVec);
            tempVec.clear();
        }
        file.close();

        // usunięcie ostatniego pustego, niepotrzebnie wczytanego wiersza
        function.pop_back();

        values.resize(function.at(0).size());
        generateNewValues(values);

        *nbOfVariables = (int) function.at(0).size();
        return true;
    } else {
        cout << "Nie można otworzyć pliku" << endl;
        return false;
    }
}

void generateFunction(vector<vector<int>> &function, vector<bool> &values, int nbOfVariables) {
    // liczba nawiasów
    int nbOfClauses = (rand() % nbOfVariables) + 5;
    function.resize(nbOfClauses);
    for (int i = 0; i < nbOfClauses; ++i) {
        function.at(i).resize(nbOfVariables);
        for (int j = 0; j < nbOfVariables; ++j) {
            function.at(i).at(j) = rand() % 4;
            /* 0 -> zwykła zmienna
             * 1 -> zanegowana
             * 2 lub 3 -> brak zmiennej
             * */
        }

        // jeżeli żadna ze zmiennych nie występuje w nawiasie...
        bool empty = true;
        for (int j = 0; j < nbOfVariables; ++j) {
            if(function.at(i).at(j) == 0 || function.at(i).at(j) == 1) empty = false;
        }
        // ... to ustawiam że pierwsza zmienna występuje w nawiasie nie zanegowana
        if(empty) {
            function.at(i).at(0) = 0;
        }
    }

    // wylosowanie pierwotnych wartości funkcji
    values.resize(nbOfVariables);
    generateNewValues(values);
}

bool calculateFunction(vector<vector<int>> &function, vector<bool> &values) {
    bool conjOfClauses = true;
    for (int i = 0; i < function.size(); ++i) {
        bool disjOfLiterals = false;
        for (int j = 0; j < function.at(i).size(); ++j) {
            if (function.at(i).at(j) == 0) {
                if (values.at(j) == true) disjOfLiterals = true;
            } else if (function.at(i).at(j) == 1) {
                if (values.at(j) == false) disjOfLiterals = true;
            }
        }
        if (!disjOfLiterals) conjOfClauses = false;
    }
    return conjOfClauses;
}

void generateNewValues(vector<bool> &values) {
    // wygenerowanie nowego zestawu wartości zmiennych
    for (int i = 0; i < values.size(); ++i) {
        values.at(i) = rand() % 2;
    }
}


bool WalkSat(vector<vector<int>> &function, vector<bool> values) {
    for (int x = 0; x < MAX_TRIES2; ++x) {
        for (int k = 0; k < MAX_TRIES1; ++k) {
            if (calculateFunction(function, values)) {
                cout << "TAK - ";
                printFunction(function, values, "definition");
                return true;
            }

            // poszukiwanie losowe nawiasu o wartości false
            vector<int> idOfFlsCls; // indexes of false clauses, indeksy tych nawiasów których wartość jest false
            for (int i = 0; i < function.size(); ++i) {
                bool valueOfClause = false; // wartość aktualnego nawiasu
                for (int j = 0; j < function.at(i).size(); ++j) {
                    if (function.at(i).at(j) == 0) {
                        if (values.at(j) == true) valueOfClause = true;
                    } else if (function.at(i).at(j) == 1) {
                        if (values.at(j) == false) valueOfClause = true;
                    }
                }
                if (!valueOfClause) idOfFlsCls.push_back(i);
            }

            // wylosowanie nawiasu do zanegowania
            int idOfCls = idOfFlsCls.at(rand() % idOfFlsCls.size());
            idOfFlsCls.clear();

            // wybranie losowo zmiennej z nawiasu do zanegowania
            int idOfVar;
            do {
                idOfVar = (int) (rand() % values.size());
            } while (function.at(idOfCls).at(idOfVar) > 1);

            // zanegowanie tej zmiennej
            values.at(idOfVar) = !(values.at(idOfVar) == true);
        }
        generateNewValues(values);
    }
    return false;
}

bool GSat(vector<vector<int>> &function, vector<bool> values) {
    for (int x = 0; x < MAX_TRIES2; ++x) {
        for (int k = 0; k < MAX_TRIES1; ++k) {
            if (calculateFunction(function, values)) {
                cout << "TAK - ";
                printFunction(function, values, "definition");
                return true;
            }

            // dla każdej zmiennej sprawdzam jaka będzie liczba nawiasów o wartości false jeśli ją zaneguję
            vector<int> vecFlsCls; // tablica pamiętająca liczbę false nawiasów dla zanegowanej każdej zmiennej
            for (int l = 0; l < values.size(); ++l) {

                // zanegowanie zmiennej o indeksie l
                values.at(l) = !(values.at(l) == true);

                // obliczenie ilośći nawiasów o wartości false
                int tempFls = 0; // number of false clauses, liczba nawiasów których wartość jest false;
                for (int i = 0; i < function.size(); ++i) {
                    bool valueOfClause = false; // wartość aktualnego nawiasu
                    for (int j = 0; j < function.at(i).size(); ++j) {
                        if (function.at(i).at(j) == 0) {
                            if (values.at(j) == true) valueOfClause = true;
                        } else if (function.at(i).at(j) == 1) {
                            if (values.at(j) == false) valueOfClause = true;
                        }
                    }
                    if (!valueOfClause) tempFls += 1;
                }
                vecFlsCls.push_back(tempFls);

                // przywrócenie zmiennej pierwotnej wartości, ponowne zanegowanie
                values.at(l) = !(values.at(l) == true);
            }

            // ostateczne wylosowanie zmiennej do zanegowania spośród tych które spowodują zfalsowanie najmniejszzej liczby nawiasów
            // odnalezienie minimum
            int min = vecFlsCls.at(0);
            for (int m = 1; m < vecFlsCls.size(); ++m) {
                if(vecFlsCls.at(m) < min) min = vecFlsCls.at(m);

            }
            // policzenie wszystkich minimów, (może być kilka bo mogą mieć tę samą wartość)
            vector<int> indxOfMins; // tablica indeksów tych zmiennych które są minimami
            for (int n = 0; n < vecFlsCls.size(); ++n) {
                if(vecFlsCls.at(n) == min) indxOfMins.push_back(n);
            }
            // wyloswanie zmiennej do zanegowania
            int varToNegate = indxOfMins.at(rand() % indxOfMins.size());

            values.at(vecFlsCls.at(varToNegate)) = !(values.at(vecFlsCls.at(varToNegate)) == true);

            vecFlsCls.clear();
        }
        generateNewValues(values);
    }
    return false;
}