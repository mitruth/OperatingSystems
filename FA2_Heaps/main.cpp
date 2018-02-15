/*  ANALYSIS
	Bottom-up startegy has O(n) complexity
	Top-down strategy has O(n*logn) complexity
	Bottom-up
		Advantage: faster
		Disadvantage: Array size must be known at the beginning
	Top-down
		Advantage: Dynamic data size
		Disadvantage: Slower
*/

#include <iostream>
#include "Profiler_linux.h"

using namespace std;

Profiler profiler("HW2");

int heap_size;

void copy (int *a, int *b, int n) {
    for (int i = 0; i < n; i++) {
        b[i] = a[i];
    }
}

void max_heapify (int *a, int i, int n) {
    int largest;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    profiler.countOperation("BottomUp", n, 1);
    if (l <= heap_size && a[l] > a[i])
        largest = l;
    else
        largest = i;

    profiler.countOperation("BottomUp", n, 1);
    if (r <= heap_size && a[r] > a[largest])
        largest = r;

    if (largest != i) {
        int aux = a[i];
        a[i] = a[largest];
        a[largest] = aux;
        profiler.countOperation("BottomUp", n, 3);
        max_heapify(a, largest, n);
    }
}

void build_heap_bottomUp (int *a, int n) { //builds heap starting from the leaves
    heap_size = n;
    for (int i = n / 2 - 1; i >= 0; i--)
        max_heapify(a, i, n);
}

void heap_increase_key(int *a, int i, int key, int n) {
    if (key < a[i]) {
        cout<<"ERROR: new key is smaller than current key"<<"\n";
    }
    profiler.countOperation("topDown", n, 1);
    a[i] = key;
    if (i <= 1) {
        profiler.countOperation("topDown", n, 1);
    }

    while (i > 0 && a[i / 2] < a[i]) { //parent[i] = i / 2
        profiler.countOperation("topDown", n, 1);
        int aux = a[i];
        a[i] = a[i / 2];
        a[i / 2] = aux;
        profiler.countOperation("topDown", n, 3);
        i = i / 2;
    }
}

void max_heap_insert(int *a, int key, int n) {
    heap_size++;
    a[heap_size] = -10000;
    profiler.countOperation("topDown", n, 1);
    heap_increase_key(a, heap_size, key, n);
}

void build_heap_topDown (int *a, int n) {
    heap_size = 0;
    for (int i = 1; i < n; i++) {
        max_heap_insert(a, a[i], n);
    }
}

int main() {

    int x[10000], xCopy[10000];

    int a[] = {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
    build_heap_bottomUp(a, 10);
    for (int i = 0; i < 10; i++) {
        cout<<a[i]<<" ";
    }

    cout<<"\n";

    int b[] = {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
    build_heap_topDown(b, 10);
    for (int i = 0; i < 10; i++) {
        cout<<b[i]<<" ";
    }

//    for (int j = 0; j < 5; j++) {
//
//        FillRandomArray(x, 10000);
//
//        for (int i = 100; i <= 10000; i = i + 100) {
//
//            copy(x, xCopy, i);
//            build_heap_bottomUp(xCopy, i);
//
//            copy(x, xCopy, i);
//            build_heap_topDown(xCopy, i);
//        }
//    }
//
//    profiler.createGroup("Bottom Up and Top Down", "BottomUp", "topDown");
//
//    profiler.showReport();

    return 0;
}
