#include <iostream>
#include <limits>
#include <omp.h>
#include <ctime>
#include <cstdlib> // For rand() and srand()
using namespace std;

// Sequential implementations
long long seqMin(const long long arr[], int n) {
    long long minVal = numeric_limits<long long>::max();
    for (int i = 0; i < n; ++i) {
        if (arr[i] < minVal) {
            minVal = arr[i];
        }
    }
    return minVal;
}

long long seqMax(const long long arr[], int n) {
    long long maxVal = numeric_limits<long long>::min();
    for (int i = 0; i < n; ++i) {
        if (arr[i] > maxVal) {
            maxVal = arr[i];
        }
    }
    return maxVal;
}

long long seqSum(const long long arr[], int n) {
    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += arr[i];
    }
    return sum;
}

double seqAverage(const long long arr[], int n) {
    long long sum = seqSum(arr, n);
    return static_cast<double>(sum) / n;
}

// Parallel implementations
long long parMin(const long long arr[], int n) {
    long long minVal = numeric_limits<long long>::max();
    #pragma omp parallel for reduction(min:minVal)
    for (int i = 0; i < n; ++i) {
        if (arr[i] < minVal) {
            minVal = arr[i];
        }
    }
    return minVal;
}

long long parMax(const long long arr[], int n) {
    long long maxVal = numeric_limits<long long>::min();
    #pragma omp parallel for reduction(max:maxVal)
    for (int i = 0; i < n; ++i) {
        if (arr[i] > maxVal) {
            maxVal = arr[i];
        }
    }
    return maxVal;
}

long long parSum(const long long arr[], int n) {
    long long sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; ++i) {
        sum += arr[i];
    }
    return sum;
}

double parAverage(const long long arr[], int n) {
    long long sum = parSum(arr, n);
    return static_cast<double>(sum) / n;
}

int main() {
    const int N = 50000; // Size of the array
    long long arr[N];

    // Initialize array with random values
    srand(time(NULL));
    for (int i = 0; i < N; ++i) {
        arr[i] = rand() % 1000; // Random values between 0 and 999
    }

    // Measure execution time for sequential operations
    clock_t seqStartTime, seqEndTime;
    double seqDuration;

    seqStartTime = clock();
    long long minSeq = seqMin(arr, N);
    seqEndTime = clock();
    seqDuration = double(seqEndTime - seqStartTime) / CLOCKS_PER_SEC;
    cout << "Sequential Min  Execution Time: " << seqDuration << " seconds" << endl;

    seqStartTime = clock();
    long long maxSeq = seqMax(arr, N);
    seqEndTime = clock();
    seqDuration = double(seqEndTime - seqStartTime) / CLOCKS_PER_SEC;
    cout << "Sequential Max  Execution Time: " << seqDuration << " seconds" << endl;

    seqStartTime = clock();
    long long sumSeq = seqSum(arr, N);
    seqEndTime = clock();
    seqDuration = double(seqEndTime - seqStartTime) / CLOCKS_PER_SEC;
    cout << "Sequential Sum  Execution Time: " << seqDuration << " seconds" << endl;

    seqStartTime = clock();
    double avgSeq = seqAverage(arr, N);
    seqEndTime = clock();
    seqDuration = double(seqEndTime - seqStartTime) / CLOCKS_PER_SEC;
    cout << "Sequential Average Execution Time: " << seqDuration << " seconds" << endl;

    // Measure execution time for parallel operations
    clock_t parStartTime, parEndTime;
    double parDuration;

    parStartTime = clock();
    long long minPar = parMin(arr, N);
    parEndTime = clock();
    parDuration = double(parEndTime - parStartTime) / CLOCKS_PER_SEC;
    cout << "Parallel Minimum algo Execution Time: " << parDuration << " seconds" << endl;

    parStartTime = clock();
    long long maxPar = parMax(arr, N);
    parEndTime = clock();
    parDuration = double(parEndTime - parStartTime) / CLOCKS_PER_SEC;
    cout <<"Parallel Maximum algo Execution Time: " << parDuration << " seconds" << endl;

    parStartTime = clock();
    long long sumPar = parSum(arr, N);
    parEndTime = clock();
    parDuration = double(parEndTime - parStartTime) / CLOCKS_PER_SEC;
    cout << "Parallel Sum  Execution Time: " << parDuration << " seconds" << endl;

    parStartTime = clock();
    double avgPar = parAverage(arr, N);
    parEndTime = clock();
    parDuration = double(parEndTime - parStartTime) / CLOCKS_PER_SEC;
    cout << "Parallel Average  Execution Time: " << parDuration << " seconds" << endl;

    return 0;
}
