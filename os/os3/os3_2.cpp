//os3_2.cpp
#include <iostream>
#include <iomanip>
#include <omp.h>

using namespace std;

const long N = 100000000;
const long BLOCK_SIZE = 3311200;

int main() {
    int thread_count;
    cout << "Enter number of threads: ";
    cin >> thread_count;
    
    if(thread_count <= 0) {
        cerr << "Error: Thread count must be positive\n";
        return 1;
    }

    // Parallel calculation
    double pi_result = 0.0;
    const double step = 1.0 / N;
    double start = omp_get_wtime();
    
    #pragma omp parallel num_threads(thread_count)
    {
        #pragma omp for schedule(dynamic, BLOCK_SIZE) reduction(+:pi_result)
        for(long i = 0; i < N; ++i) {
            double x = (i + 0.5) * step;
            pi_result += 4.0 / (1.0 + x * x);
        }
    }
    
    // Results output
    double elapsed = (omp_get_wtime() - start) * 1000;
    
    cout << fixed << setprecision(15);
    cout << "Result = " << pi_result / N << endl;
    cout << "Time: " << elapsed << " ms" << endl;
    cin.get();
    cin.get();
    return 0;
}