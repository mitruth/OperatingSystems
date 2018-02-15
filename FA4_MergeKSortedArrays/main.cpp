#include <iostream>
#include "Profiler_linux.h"

Profiler profiler ("merge_k_sorted_arrays");

using namespace std;

struct element {
    int listIndex;
    int value;
    element *next;
};

struct listK {
    int length;
    element *first;
    element *last;
};

listK elements[1000];

int k, heapSize, result[10000001], resultLength, totalOperations, demoArray[10002], x[10002];
int n;

void addElement (int index, int value) {
    element *x = new element;
    x->listIndex = index;
    x->value = value;
    if (elements[index].length == 0) {
        elements[index].first = elements[index].last = x;
    } else {
        elements[index].last->next = x;
        elements[index].last = x;
    }
    elements[index].length++;
}

void min_heapify (int position) {
    int min;
    int l = 2 * position + 1;
    int r = 2 * position + 2;
    totalOperations++;
    if (l <= heapSize && elements[l].first->value < elements[position].first->value) {
        min = l;
    } else {
        min = position;
    }
    totalOperations++;
    if (r <= heapSize && elements[r].first->value < elements[min].first->value) {
        min = r;
    }
    if (min != position) {
        listK aux;
        totalOperations += 3;
        aux = elements[position];
        elements[position] = elements[min];
        elements[min] = aux;
        totalOperations++;
        min_heapify(min);
    }
}

void build_heap_bottomUp() {

    for (int i = heapSize / 2; i >= 0; i--) {
        min_heapify(i);
    }
}

void mainOperation() {
    totalOperations = 0;
    int numberOfElements = 0;
    heapSize = k - 1;
    build_heap_bottomUp();
    for (int i = 0; i < k; i++) {
        numberOfElements += elements[i].length;
    }
    resultLength = 0;
    while (numberOfElements != 0) {
        totalOperations++;
        result[resultLength++] = elements[0].first->value;
        element *x;
        totalOperations++;
        x = elements[0].first;
        elements[0].length--;
        if (elements[0].length != 0) {
            elements[0].first = elements[0].first->next;
            totalOperations++;
        } else {
            listK aux;
            aux = elements[0];
            elements[0] = elements[heapSize];
            elements[heapSize] = aux;
            totalOperations += 3;
            heapSize--;
        }
        free(x);
        numberOfElements--;
        min_heapify(0);
    }
}

int main() {

    int array_size = 0;
    //demo for proving the corectness of the algorithm
    k = 5;
    cout<< k << endl;
    for (int i = 0; i < k; i++) {
        int m = 2 + rand() % 8;
        FillRandomArray(demoArray, m, 0, 10, false, 1);

        cout << m << " elements in list " << i << " : ";
        for (int j = 0; j < m; j++) {
            addElement(i, demoArray[j]);
            cout << demoArray[j] << ' ';
        }
        cout << endl;
    }
    cout << endl << "Merged arrays " << endl;
    mainOperation();

    for (int i = 0; i < resultLength; i++) {
        cout << result[i] << ' ';
    }

//    //profiler operations
//    k = 5;
//    for (n = 100; n <= 10000; n += 100) {
//        for (int i = 0; i < k; i++) {
//            array_size = n / k;
//            FillRandomArray(x, array_size, 0, 15000, false, 1);
//            for (int j = 0; j < array_size; j++) {
//                addElement(i, x[i]);
//            }
//        }
//        mainOperation();
//        profiler.countOperation("When k=5", n, totalOperations);
//    }
//    //////////////////////////////////////////////
//    k = 10;
//    for (n = 100; n <= 10000; n += 100) {
//        for (int i = 0; i < k; i++) {
//            array_size = n / k;
//            FillRandomArray(x, array_size, 0, 15000, false, 1);
//            for (int j = 0; j < array_size; j++) {
//                addElement(i, x[j]);
//            }
//        }
//        mainOperation();
//        profiler.countOperation("When k=10", n, totalOperations);
//    }
//    //////////////////////////////////////////////
//    k = 100;
//    for (n = 100; n <= 10000; n += 100) {
//        for (int i = 0; i < k; i++) {
//            array_size = n / k;
//            FillRandomArray(x, array_size, 0, 15000, false, 1);
//            for (int j = 0; j < array_size; j++) {
//                addElement(i, x[j]);
//            }
//        }
//        mainOperation();
//        profiler.countOperation("When k=100", n, totalOperations);
//    }
//    /////////////////////////////////////////////
//    n = 10000;
//    for (k = 10; k <= 500; k = k + 10) {
//        for (int i = 0; i < k; i++) {
//            array_size = n / k;
//            FillRandomArray(x, array_size, 0, 15000, true, 1);
//            for (int j = 0; j < array_size; j++) {
//                addElement(i, x[j]);
//            }
//        }
//        mainOperation();
//        profiler.countOperation("When n=10000", k, totalOperations);
//    }
//
//    profiler.createGroup("Comparison when k belongs to {5, 10, 100}", "When k=5", "When k=10", "When k=100");
//    profiler.createGroup("Fixed n", "When n=10000");
//    profiler.showReport();

    return 0;
}