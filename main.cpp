#include <iostream>
using namespace std;

int main()
{
    cout<<"Hi there !!"<<endl;

    return 0;
}

// onkardpatil07@cloudshell:~/cloud_comp (isentropic-now-456017-p2)$ g++ main.cpp -o main
// onkardpatil07@cloudshell:~/cloud_comp (isentropic-now-456017-p2)$ ./main

// #include <iostream>
// using namespace std;

// void selectionSort(int arr[], int n) {
//     for (int i = 0; i < n - 1; i++) {
//         int minIndex = i;
//         for (int j = i + 1; j < n; j++) {
//             if (arr[j] < arr[minIndex]) {
//                 minIndex = j;
//             }
//         }
//         if (minIndex != i) {
//             swap(arr[i], arr[minIndex]);
//         }
//     }
// }

// void printArray(int arr[], int n) {
//     for (int i = 0; i < n; i++)
//         cout << arr[i] << " ";
//     cout << endl;
// }

// int main() {
//     int n;
//     cout << "Enter number of elements: ";
//     cin >> n;

//     int arr[n];
//     cout << "Enter " << n << " elements: ";
//     for (int i = 0; i < n; i++)
//         cin >> arr[i];

//     cout << "Original array: ";
//     printArray(arr, n);

//     selectionSort(arr, n);

//     cout << "Sorted array: ";
//     printArray(arr, n);

//     return 0;
// }
