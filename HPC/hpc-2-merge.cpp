#include <iostream>
#include <ctime>
#include <cstdlib> // For rand() and srand()
#include <omp.h>

using namespace std;

const int THRESHOLD = 10000; // Threshold for parallel execution

// Sequential merge function
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (int i = 0; i < n1; ++i)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

// Sequential merge sort function
void mergeSortSequential(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortSequential(arr, l, m);
        mergeSortSequential(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Parallel merge sort function
void mergeSortParallel(int arr[], int l, int r) {
    if (l < r) {
        if (r - l < THRESHOLD) {
            mergeSortSequential(arr, l, r); // Use sequential merge sort for small input sizes
        } else {
            int m = l + (r - l) / 2;
            #pragma omp parallel sections
            {
                #pragma omp section
                {
                    mergeSortParallel(arr, l, m);
                }
                #pragma omp section
                {
                    mergeSortParallel(arr, m + 1, r);
                }
            }
            merge(arr, l, m, r);
        }
    }
}

int main() {
    const int N = 1000; // Size of the array
    int arrSequential[N];
    int arrParallel[N];

    // Initialize arrays with random values
    srand(time(NULL));
    for (int i = 0; i < N; ++i) {
        arrSequential[i] = rand() % 1000; // Random values between 0 and 999
        arrParallel[i] = arrSequential[i];
    }

    // Measure execution time for sequential merge sort
    clock_t startSeq = clock();
    mergeSortSequential(arrSequential, 0, N - 1);
    clock_t stopSeq = clock();
    double durationSeq = (double)(stopSeq - startSeq) / CLOCKS_PER_SEC;
    cout << "Sequential Merge Sort Time: " << durationSeq << " seconds" << endl;

    // Measure execution time for parallel merge sort
    clock_t startPar = clock();
    mergeSortParallel(arrParallel, 0, N - 1);
    clock_t stopPar = clock();
    double durationPar = (double)(stopPar - startPar) / CLOCKS_PER_SEC;
    cout << "Parallel Merge Sort Time: " << durationPar << " seconds" << endl;

    return 0;
}
