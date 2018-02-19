#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Item
{
    int value;
    int weight;

    int index;
    string name;
};

void coutIntVector(vector<int> *vec);
int dispMenu();
bool loadData(int choice, int* n, int* c, vector<Item>* items);
void solveKnapsack(int n, int c, vector<Item>* items);

// warunki sortowań vectorów
bool sortResultAsc(const int &lhs, const int &rhs) { return lhs < rhs; }
bool sortbyValueDesc(const Item &lhs, const Item &rhs) { return lhs.value > rhs.value; }
bool sortbyWeightAsc(const Item &lhs, const Item &rhs) { return lhs.weight < rhs.weight; }
bool sortbyRatioDesc(const Item &lhs, const Item &rhs) {
    float lratio = lhs.value/lhs.weight;
    float rratio = rhs.value/rhs.weight;
    return lratio > rratio;
}

int main() {
    int choice;

    int capacity = 0;  // pojemność plecaka
    int nOfElements = 0;  // liczba elementów
    vector<Item> items;

    choice = dispMenu();
    bool dataOK = loadData(choice, &nOfElements, &capacity, &items);
    if(dataOK) solveKnapsack(nOfElements, capacity, &items);
    else
    {
        cout << "Błąd";
        items.clear();
        return -1;
    }

    items.clear();
    return 0;
}

void solveKnapsack(int n, int c, vector<Item>* items)
{
    int values[3] = {0, 0, 0}; // suma wartości z poszczególnych metod
    int weights[3] = {0, 0, 0}; // suma wag z poszczególnych metod

    int tempWeight;
    vector<int> by1Weight;
    vector<int> by2Value;
    vector<int> by3Ratio;

    // pod względem wagi
    tempWeight = 0;
    sort(items->begin(), items->end(), sortbyWeightAsc);
    for(int i=0; i<n; i++)
    {
        if(tempWeight + items->at(i).weight <= c)
        {
            tempWeight += items->at(i).weight;
            by1Weight.push_back(items->at(i).index);
            values[0] += items->at(i).value;
            weights[0] += items->at(i).weight;
        }
        else break;
    }

    // pod względem wartości
    tempWeight = 0;
    sort(items->begin(), items->end(), sortbyValueDesc);
    for(int i=0; i<n; i++)
    {
        if(tempWeight + items->at(i).weight <= c)
        {
            tempWeight += items->at(i).weight;
            by2Value.push_back(items->at(i).index);
            values[1] += items->at(i).value;
            weights[1] += items->at(i).weight;
        }
    }

    // pod względem ratio
    tempWeight = 0;
    sort(items->begin(), items->end(), sortbyRatioDesc);
    for(int i=0; i<n; i++)
    {
        if(tempWeight + items->at(i).weight <= c)
        {
            tempWeight += items->at(i).weight;
            by3Ratio.push_back(items->at(i).index);
            values[2] += items->at(i).value;
            weights[2] += items->at(i).weight;
        }
    }

    sort(by1Weight.begin(), by1Weight.end(), sortResultAsc);
    sort(by2Value.begin(), by2Value.end(), sortResultAsc);
    sort(by3Ratio.begin(), by3Ratio.end(), sortResultAsc);

    cout << "Należy spakować przedmioty: ";
    if(values[0] <= values[1] && values[1] <= values[2])
    {
        coutIntVector(&by3Ratio);
        cout << endl << "Całkowita wartość: " << values[2];
        cout << endl << "Całkowita waga: " << weights[2];
    }
    else if(values[1] <= values[2] && values[2] <= values[0])
    {
        coutIntVector(&by1Weight);
        cout << endl << "Całkowita wartość: " << values[0];
        cout << endl << "Całkowita waga: " << weights[0];
    }
    else if(values[0] <= values[2] && values[2] <= values[1])
    {
        coutIntVector(&by2Value);
        cout << endl << "Całkowita wartość: " << values[1];
        cout << endl << "Całkowita waga: " << weights[1];
    }

    by1Weight.clear();
    by2Value.clear();
    by3Ratio.clear();
}

bool loadData(int choice, int* n, int* c, vector<Item>* items)
{
    if(choice == 1)
    {
        string address;
        cout << "Podaj adres do pliku z rozszerzeniem: ";
        cin >> address;
        int nTemp = 0; // liczba elementów
        int cTemp = 0; // pojemność plecaka

        fstream file;
        file.open(address, ios::in);
        if(file.fail()) return false;
        else
        {
            char tempChar = 0;
            int tempInt = 0;

            // wczytanie liczby elemntów
            file.get(tempChar);
            while(tempChar != ' ')
            {
                nTemp = nTemp*10 + tempChar - 48;
                file.get(tempChar);
            }

            items->resize(nTemp);

            // wczytanie pojemności plecaka
            file.get(tempChar);
            while(tempChar != '\n')
            {
                cTemp = cTemp*10 + tempChar - 48;
                file.get(tempChar);
            }


            // wczytanie kolejnych elemntów
            for(int i=0; i<nTemp; i++)
            {
                file.get(tempChar);
                while(tempChar != ' ')
                {
                    tempInt = tempInt*10 + tempChar - 48;
                    file.get(tempChar);
                }
                items->at(i).value = tempInt;
                tempInt = 0;
                file.get(tempChar);
                while(tempChar != '\n')
                {
                    tempInt = tempInt*10 + tempChar - 48;
                    file.get(tempChar);
                }
                items->at(i).weight = tempInt;
                items->at(i).index = i;
                tempInt = 0;
            }

        }
        file.close();

        *n = nTemp;
        *c = cTemp;

        return true;
    }
    else if (choice == 2)
    {
        int nTemp;
        int cTemp;

        cout<<"Pojemność plecaka: ";
        cin>>cTemp;
        if(cTemp<1)
        {
            cout<<"pojemność plecaka nie może wynosić "<<cTemp<<endl;
            return false;
        }
        cout<<"Liczba elementów: ";
        cin>>nTemp;
        if(nTemp<1)
        {
            cout<<"liczba elementów nie może wynosić "<<nTemp<<endl;
            return false;
        }
        cout<<endl;

        items->resize(nTemp);

        cout<<"Wartości i wagi kolejnych elementów: "<<endl;
        for(int i=0; i<nTemp; i++)
        {
            int temp = 0;
            while(temp<1)
            {
                cout<<i+1<<". wartość: ";
                cin>>temp;
                if(temp<1) cout<<"wartość musi być większa od zera"<<endl;
                else items->at(i).value = temp;
            }
            temp = 0;
            while(temp<1)
            {
                cout<<"   waga: ";
                cin>>temp;
                if(temp<1) cout<<"waga musi być większa od zera"<<endl;
                else items->at(i).weight = temp;
            }
            items->at(i).index = i;
        }

        *n = nTemp;
        *c = cTemp;
        return true;
    }

    else return false;
}

int dispMenu()
{
    cout << "PROBLEM PLECAKOWY - ALGORYTM ZACHŁANNY" << endl << endl;
    cout << "1. Wczytaj elementy z pliku" << endl;
    cout << "2. Wpisz z klawiatury" << endl;

    int choice=0;
    while(choice!=1 && choice != 2)
    {
        cout << "Wybieram: ";
        cin >> choice;
    }
    cout << endl;
    return choice;
}

void coutIntVector(vector<int> *vec)
{
    for(vector<int>::iterator iter = vec->begin(); iter != vec->end(); iter++)
    {
        cout << *iter + 1 << " ";
    }
}