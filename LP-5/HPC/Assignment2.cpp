// Title - Parallel Sorting Algorithms
// Problem Statement - Write a program to implement Parallel Bubble Sort and Merge sort using OpenMP. Use existing algorithms and measure the performance of sequential and parallel algorithms.

#include<iostream>
#include<vector>
#include<omp.h>

using namespace std;

void merge(vector<int>& arr, int left, int mid, int right){
    int n1 = mid - left + 1;
    vector<int> arrLeft;
    for(int i=0; i< n1; i++){
        arrLeft.push_back(arr[left+i]);
    }

    int n2 = right - mid;
    vector<int> arrRight;
    for(int i=0; i<=n2; i++){
        arrRight.push_back(arr[mid+1+i]);
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2){
        if(arrLeft[i] <= arrRight[j]){
            arr[k] = arrLeft[i];
            i++;
        }else{
            arr[k] = arrRight[j];
            j++;
        }
        k++;
    }

    while(i < n1){
        arr[k] = arrLeft[i];
        i++;
        k++;
    }

    while(j < n2){
        arr[k] = arrRight[j];
        j++;
        k++;
    }
}

void sequentialMergeSort(vector<int>& arr, int left, int right){
    if (left < right) {
        int mid = left + (right - left) / 2;

        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid+1, right);

        merge(arr, left, mid, right);
    }
}

void parallelMergeSort(vector<int>& arr, int left, int right){
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                parallelMergeSort(arr, left, mid);
            }
            #pragma omp section
            {
                parallelMergeSort(arr, mid+1, right);
            }
        }

        merge(arr, left, mid, right);
    }
}

void sequentialBubbleSort(vector<int>& arr, int& n){
    for(int i=0; i<n-1; i++){
        for(int j=0; j<n-i-1; j++){
            if(arr[j] > arr[j+1]){
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

void parallelBubbleSort(vector<int>& arr, int& n){
    for(int i=0; i<n; i++){
        int phase = i%2;

        #pragma omp parallel for default(none) shared(arr, n, phase)
        for(int j=phase; j<n-1; j+=2){
            if(arr[j] > arr[j+1]){
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

int main() {
    int n = 1000000;

    vector<int> arr;
    for(int i=0; i<n; i++){
        arr.push_back(rand() % n);
    }

    vector<int> arr1 = arr;
    auto start = omp_get_wtime();
    sequentialMergeSort(arr1, 0, n);
    auto stop = omp_get_wtime();
    auto duration = stop - start;
    cout << "Time Taken By Sequential Merge Sort : " << duration << endl;

    vector<int> arr2 = arr;
    start = omp_get_wtime();
    parallelMergeSort(arr2, 0, n);
    stop = omp_get_wtime();
    duration = stop - start;
    cout << "Time Taken By Parallel Merge Sort : " << duration << endl;

    vector<int> arr3 = arr;
    start = omp_get_wtime();
    sequentialBubbleSort(arr3, n);
    stop = omp_get_wtime();
    duration = stop - start;
    cout << "Time Taken By Sequential Bubble Sort : " << duration << endl;

    vector<int> arr4 = arr;
    start = omp_get_wtime();
    parallelBubbleSort(arr4, n);
    stop = omp_get_wtime();
    duration = stop - start;
    cout << "Time Taken By Parallel Bubble Sort : " << duration << endl;

    return 0;
}

// Run Program -
// Compile Command - g++ -fopenmp Assignment2.cpp -o assignment2
// Execute Command - ./assignment2