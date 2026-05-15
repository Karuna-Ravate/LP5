// Code with Thread ID Printing
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
        int thread_id = omp_get_thread_num();

        #pragma omp critical
        {
            cout << "Thread ID = " << thread_id << " processing index = " << i
                 << " value = " << arr[i] << endl;
        }

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
// Enter number of elements: 10
// Enter 10 elements:
// 1 2 3 4 5 6 7 8 9 10
// Enter number of threads: 4

// //o/p
// Thread ID = 0 processing index = 0 value = 1
// Thread ID = 0 processing index = 1 value = 2
// Thread ID = 0 processing index = 2 value = 3
// Thread ID = 1 processing index = 3 value = 4
// Thread ID = 1 processing index = 4 value = 5
// Thread ID = 1 processing index = 5 value = 6
// Thread ID = 2 processing index = 6 value = 7
// Thread ID = 2 processing index = 7 value = 8
// Thread ID = 3 processing index = 8 value = 9
// Thread ID = 3 processing index = 9 value = 10

// ========== Parallel Reduction Result ==========
// Minimum Value : 1
// Maximum Value : 10
// Sum           : 55
// Average       : 5.5
// Execution Time: 0.000412 seconds

// Program executed successfully.