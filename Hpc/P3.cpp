#include <iostream>
#include <vector>
#include <omp.h>
#include <limits>

using namespace std;

int main() {
    int n, threads;

    cout << "Enter number of elements: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid number of elements." << endl;
        return 0;
    }

    vector<double> arr(n);

    cout << "Enter " << n << " elements:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    cout << "Enter number of threads: ";
    cin >> threads;

    if (threads <= 0) {
        cout << "Invalid number of threads." << endl;
        return 0;
    }

    omp_set_num_threads(threads);

    double min_val = numeric_limits<double>::max();
    double max_val = numeric_limits<double>::lowest();
    double sum = 0.0;

    double start_time = omp_get_wtime();

    #pragma omp parallel for reduction(min:min_val) reduction(max:max_val) reduction(+:sum)
    for (int i = 0; i < n; i++) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }

        if (arr[i] > max_val) {
            max_val = arr[i];
        }

        sum += arr[i];
    }

    double end_time = omp_get_wtime();

    double average = sum / n;

    cout << "\n========== Parallel Reduction Result ==========" << endl;
    cout << "Minimum Value : " << min_val << endl;
    cout << "Maximum Value : " << max_val << endl;
    cout << "Sum           : " << sum << endl;
    cout << "Average       : " << average << endl;
    cout << "Execution Time: " << end_time - start_time << " seconds" << endl;

    cout << "\nProgram executed successfully." << endl;

    return 0;
}
//run
// g++ -fopenmp parallel_reduction.cpp -o P3.exe
//P5.exe

// Enter number of elements: 10
// Enter 10 elements:
// 1 2 3 4 5 6 7 8 9 10
// Enter number of threads: 4

// //o/p
// ========== Parallel Reduction Result ==========
// Minimum Value : 1
// Maximum Value : 10
// Sum           : 55
// Average       : 5.5
// Execution Time: 0.000234 seconds

// Program executed successfully.