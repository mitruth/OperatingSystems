#include <iostream>
#include "iomanip"
#include "Profiler_linux.h"

Profiler profiler ("insert_search_hashTable");
//one of the best choices for these constants such that the hash function has different
//results for i in [0, (m - 1) / 2]
const int c1 = 0;
const int c2 = 1;

using namespace std;

int hashTable[9974];
int n = 9973;

void initialize() {
    for (int i = 0; i < n; i++) {
        hashTable[i] = 0; //we initialize all the elements from the table with 0
    }
}

int hashFunction(int key, int i) {
    int aux;
    aux = key % n;
    return (aux + c1 * i + c2 * i * i) % n;
}

int searchHash (int key) {
    int positionsAccessed = 0;
    int i = 0;
    int j;
    do {
        j = hashFunction(key, i);
        positionsAccessed++;
        if (hashTable[j] == key) {
            return positionsAccessed;
        }
        i++;
    } while (i < n && hashTable[j] != 0);
    return positionsAccessed;
}

int insertHash (int key) {
    int i = 0;
    int j;
    do {
        j = hashFunction(key, i);
        if (hashTable[j] == 0) {
            hashTable[j] = key;
            return j;
        }
        i++;
    }while (i < n);
    return 0;
}

void fillTable (float fillingFactor) {

    int nrElements = 0;
    float a = 0; //alpha, filling factor of the hash table
    int j = 0;
    int i = 0;
    float avgEffortFound = 0;
    float avgEffortNotFound = 0;
    int maxEffortFound = 0;
    int maxEffortNotFound = 0;
    int totalEffortFound = 0;
    int totalEffortNotFound = 0;

    int search[1500];
    int searchLength = 1500;

    initialize();

    while (a < fillingFactor) {
        int randomNr = 1+(123*rand()) % 100000;//we want to generate numbers to be found, different than the numbers that we do not want to find (to calculate
        // the effort for not finding), so we take n (size of table) numbers, from n to 2*n
        if (insertHash(randomNr) != 0) {
            nrElements++;
        }
        a = (float)nrElements / n; //we fill the table with values until the filling factor is the desired one
    }
   j=0;
    while (i < n && j < searchLength) { //puts all inserted numbers
        //into the array "search"
        if (hashTable[i] != 0) {
            search[j] = hashTable[i];
            j++;
        }
        i += 5;//in order to make it more random, we do not want to search for the first 1500 numbers
    }

    for (i = 0; i < searchLength; i++) { //calculates the effort when searching each
        //inserted element
        int slotsAccessed = searchHash(search[i]);
        totalEffortFound += slotsAccessed;
        if (slotsAccessed > maxEffortFound) {
            maxEffortFound = slotsAccessed;
        }
    }

    avgEffortFound = (float)totalEffortFound / j;

    for (i = 0; i < searchLength; i++) { //generate random numbers from the range 1 to 9973, so that we are sure that they are all different than
        //the elements from the hash table and we can not find them
        int randomNr2 = 100000 + rand();
        int slotsAccessed = searchHash(randomNr2);
        totalEffortNotFound += slotsAccessed;
        if (slotsAccessed > maxEffortNotFound) {
            maxEffortNotFound = slotsAccessed;
        }
    }

    avgEffortNotFound = (float)totalEffortNotFound / searchLength;

    cout<< fixed;
    cout<< setprecision(2);

    cout<< "     " << a << "               " << avgEffortFound << "                 " << maxEffortFound << "                    " << avgEffortNotFound << "                    " << maxEffortNotFound << endl;
}

int main() {

//    int positionsAccessed = 0;
//
//    initialize();
//    insertHash(5);
//    insertHash(13);
//    insertHash(124);
//    insertHash(1460);
//    for (int i = 0; i < n; i++) {
//        if (hashTable[i] != 0) {
//            cout<<"Element " << hashTable[i] << " on the position: " << i << endl;
//        }
//    }
//    positionsAccessed = searchHash(13);
//    cout << positionsAccessed << " is the number of positions accessed before finding the number 13" << endl;
//    positionsAccessed = searchHash(10);
//    cout << positionsAccessed << " is the number of positions accessed for finding a non existing elements in the table (10)" << endl;
//
//    cout << endl;

    cout<< "Filling Factor  Average Effort Found  Max Effort Found  Average Effort Not Found  Max Effort Not Found" << endl;
    fillTable(0.8);
    fillTable(0.85);
    fillTable(0.9);
    fillTable(0.95);
    fillTable(0.99);
    return 0;
}