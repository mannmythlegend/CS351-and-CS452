#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

void merge(int* a, int* b, int lasta, int lastb, int* output = NULL) {
    int i=0, j=0, k = 0;
    int n = lasta + lastb + 2;
    int*c = new int[n];
	
	//main merge
    while (i <= lasta && j <= lastb) {
        if (a[i] <= b[j]) {
            c[k++] = a[i++];
        } else {
            c[k++] = b[j++];
        }
    }

    //one of arrays will have elements remaining. fill whichever loop runs
    while (i <= lasta) {
        c[k++] = a[i++];
    }
    while (j <= lastb) {
        c[k++] = b[j++];
    }

    //copy answer to original array
    for (int x = 0; x < n; ++x) {
        a[x] = c[x];
    }

    delete[] c;
}

void mergesort(int* a, int first, int last) {
    if (first < last) {
		//midpoint for all recursed scenarios
        int midpoint = (first+last) / 2;
		
		//recursion
        mergesort(a, first, midpoint);
        mergesort(a, midpoint+1, last);
		
		//merge
        merge(a+first, a+midpoint+1, midpoint-first, last-midpoint-1);
    }
}

int main() {
    int n =0;
    cout << "Enter the size of the array: ";
    cin >> n;
    int* a = new int[n];

    // rng seed
    srand(time(0));

    //Fill array with random numbers
    for (int i=0; i<n; i++) {
        a[i] = rand() % 1000001;
    }

    //print original array
    std::cout << "Original Array:" << endl;
    for (int i=0; i<n; i++) {
        cout << a[i] << " ";
    }
    cout << endl;

    //mergesort
    mergesort(a, 0, n-1);

    //sorted array
    cout << "Sorted Array:" << endl;
    for (int i=0; i<n; i++) {
        cout << a[i] << " ";
    }
    cout << endl;

    delete[] a;
    return 0;
}

