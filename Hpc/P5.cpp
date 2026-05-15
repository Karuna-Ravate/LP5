#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <omp.h>

using namespace std;

const int FEATURES = 5;

// Structure to store person data
struct Person {
    double values[FEATURES];
    int cluster;
};

// Function to calculate Euclidean distance
double calculateDistance(Person p, vector<double> centroid) {
    double sum = 0.0;

    for (int i = 0; i < FEATURES; i++) {
        sum += pow(p.values[i] - centroid[i], 2);
    }

    return sqrt(sum);
}

// Function to display cluster meaning
void displayClusterMeaning(int cluster) {
    if (cluster == 0) {
        cout << "Low Risk";
    } else if (cluster == 1) {
        cout << "Medium Risk";
    } else {
        cout << "High Risk";
    }
}

int main() {
    int n, k, maxIterations;

    cout << "Enter number of people/data points: ";
    cin >> n;

    cout << "Enter number of clusters: ";
    cin >> k;

    cout << "Enter maximum number of iterations: ";
    cin >> maxIterations;

    vector<Person> people(n);
    vector<vector<double>> centroids(k, vector<double>(FEATURES));

    cout << "\nEnter data for each person:\n";
    cout << "Format: Age Symptoms ContactCount ComorbidityCount TravelHistoryScore\n";
    cout << "Example: 45 3 8 2 1\n\n";

    for (int i = 0; i < n; i++) {
        cout << "Enter data for person " << i + 1 << ": ";
        for (int j = 0; j < FEATURES; j++) {
            cin >> people[i].values[j];
        }
        people[i].cluster = -1;
    }

    // Initialize centroids using first k data points
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < FEATURES; j++) {
            centroids[i][j] = people[i].values[j];
        }
    }

    double startTime = omp_get_wtime();

    for (int iteration = 0; iteration < maxIterations; iteration++) {
        bool changed = false;

        // Parallel cluster assignment
        #pragma omp parallel for shared(changed)
        for (int i = 0; i < n; i++) {
            double minDistance = numeric_limits<double>::max();
            int nearestCluster = -1;

            for (int c = 0; c < k; c++) {
                double distance = calculateDistance(people[i], centroids[c]);

                if (distance < minDistance) {
                    minDistance = distance;
                    nearestCluster = c;
                }
            }

            if (people[i].cluster != nearestCluster) {
                people[i].cluster = nearestCluster;
                changed = true;
            }
        }

        // Create temporary arrays for centroid update
        vector<vector<double>> newCentroids(k, vector<double>(FEATURES, 0.0));
        vector<int> clusterCount(k, 0);

        // Sum values for each cluster
        for (int i = 0; i < n; i++) {
            int cluster = people[i].cluster;
            clusterCount[cluster]++;

            for (int j = 0; j < FEATURES; j++) {
                newCentroids[cluster][j] += people[i].values[j];
            }
        }

        // Calculate new centroids
        for (int c = 0; c < k; c++) {
            if (clusterCount[c] != 0) {
                for (int j = 0; j < FEATURES; j++) {
                    centroids[c][j] = newCentroids[c][j] / clusterCount[c];
                }
            }
        }

        if (!changed) {
            cout << "\nAlgorithm converged at iteration " << iteration + 1 << endl;
            break;
        }
    }

    double endTime = omp_get_wtime();

    cout << "\n========== Final Clustering Result ==========\n";

    for (int i = 0; i < n; i++) {
        cout << "Person " << i + 1 << " -> Cluster " << people[i].cluster
             << " (";

        displayClusterMeaning(people[i].cluster);

        cout << ")" << endl;
    }

    cout << "\n========== Final Centroids ==========\n";

    for (int c = 0; c < k; c++) {
        cout << "Cluster " << c << " centroid: ";

        for (int j = 0; j < FEATURES; j++) {
            cout << centroids[c][j] << " ";
        }

        cout << endl;
    }

    cout << "\nExecution Time using OpenMP: "
         << endTime - startTime << " seconds" << endl;

    cout << "\nProgram executed successfully.\n";

    return 0;
}

//run

// g++ -fopenmp hpc_ai_ml_kmeans.cpp -o P5.exe
//P5.exe

// Enter number of people/data points: 6
// Enter number of clusters: 3
// Enter maximum number of iterations: 10

// Enter data for person 1: 22 1 2 0 0
// Enter data for person 2: 25 1 3 0 0
// Enter data for person 3: 45 3 8 1 1
// Enter data for person 4: 50 4 10 2 1
// Enter data for person 5: 70 5 15 3 1
// Enter data for person 6: 75 6 18 4 1


//o/p
// Algorithm converged at iteration 2

// ========== Final Clustering Result ==========
// Person 1 -> Cluster 0 (Low Risk)
// Person 2 -> Cluster 0 (Low Risk)
// Person 3 -> Cluster 1 (Medium Risk)
// Person 4 -> Cluster 1 (Medium Risk)
// Person 5 -> Cluster 2 (High Risk)
// Person 6 -> Cluster 2 (High Risk)

// ========== Final Centroids ==========
// Cluster 0 centroid: 23.5 1 2.5 0 0
// Cluster 1 centroid: 47.5 3.5 9 1.5 1
// Cluster 2 centroid: 72.5 5.5 16.5 3.5 1

// Execution Time using OpenMP: 0.000123 seconds

// Program executed successfully.



