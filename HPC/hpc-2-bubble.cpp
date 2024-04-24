#include <iostream>
#include <ctime>
#include <omp.h>
#include <cstdlib>

using namespace std;

// Sequential Bubble Sort
void bubbleSortSequential(int arr[], int n) {
    for (int i = 0; i < n-1; ++i) {
        for (int j = 0; j < n-i-1; ++j) {
            if (arr[j] > arr[j+1]) {
                // Swap arr[j] and arr[j+1]
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// Parallel Bubble Sort
void bubbleSortParallel(int arr[], int n) {
    #pragma omp parallel
    {
        for (int i = 0; i < n-1; ++i) {
            #pragma omp for
            for (int j = 0; j < n-i-1; ++j) {
                if (arr[j] > arr[j+1]) {
                    // Swap arr[j] and arr[j+1]
                    int temp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                }
            }
        }
    }
}

int main() {
    const int N = 10000; // Size of the array
    int arr[N];

    // Initialize array with random values
    srand(time(NULL));
    for (int i = 0; i < N; ++i) {
        arr[i] = rand() % 1000; // Random values between 0 and 999
    }

    // Sequential Bubble Sort
    clock_t startSeq = clock();
    bubbleSortSequential(arr, N);
    clock_t stopSeq = clock();
    double durationSeq = (double)(stopSeq - startSeq) / CLOCKS_PER_SEC;
    cout << "Sequential Bubble Sort Time: " << durationSeq << " seconds" << endl;

    // Initialize array with random values again for parallel sort
    for (int i = 0; i < N; ++i) {
        arr[i] = rand() % 1000; // Random values between 0 and 999
    }

    // Parallel Bubble Sort
    clock_t startPar = clock();
    bubbleSortParallel(arr, N);
    clock_t stopPar = clock();
    double durationPar = (double)(stopPar - startPar) / CLOCKS_PER_SEC;
    cout << "Parallel Bubble Sort Time: " << durationPar << " seconds" << endl;

    return 0;
}
