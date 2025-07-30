//os3_1.cpp
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

const long N = 100000000;
const long BLOCK_SIZE = 3311200;

double pi_result = 0.0;
HANDLE pi_mutex;
int thread_count;
vector<bool> is_working;
vector<HANDLE> thread_pool;
long next_block = 0;

struct ThreadData {
    int id;
    long start;
    long end;
};

vector<ThreadData> thread_data;

DWORD WINAPI CalculatePi(LPVOID param) {
    ThreadData* data = (ThreadData*)param;
    
    while(true) {
        double partial_sum = 0.0;
        double step = 1.0 / N;
        
        for(long i = data->start; i < data->end && i < N; ++i) {
            double x = (i + 0.5) * step;
            partial_sum += 4.0 / (1.0 + x * x);
        }
        
        WaitForSingleObject(pi_mutex, INFINITE);
        pi_result += partial_sum;
        is_working[data->id] = false;
        ReleaseMutex(pi_mutex);
        
        SuspendThread(GetCurrentThread());
        
        if(data->start >= N) break;
    }
    return 0;
}

int main() {
    cout << "Enter number of threads: ";
    cin >> thread_count;

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    pi_mutex = CreateMutex(NULL, FALSE, NULL);
    is_working.resize(thread_count, false);
    thread_pool.resize(thread_count);
    thread_data.resize(thread_count);
    
    // Create suspended threads with initial work
    for(int i = 0; i < thread_count; ++i) {
        thread_data[i].id = i;
        thread_data[i].start = next_block;
        thread_data[i].end = (next_block += BLOCK_SIZE);
        is_working[i] = true;
        
        thread_pool[i] = CreateThread(NULL, 0, CalculatePi, &thread_data[i], CREATE_SUSPENDED, NULL);
        ResumeThread(thread_pool[i]);
    }
    
    // Main work distribution loop
    while(true) {
        bool all_done = true;
        
        WaitForSingleObject(pi_mutex, INFINITE);
        for(int i = 0; i < thread_count; ++i) {
            if(!is_working[i] && next_block < N) {
                thread_data[i].start = next_block;
                thread_data[i].end = (next_block += BLOCK_SIZE);
                is_working[i] = true;
                ResumeThread(thread_pool[i]);
            }
            
            if(is_working[i] || next_block < N) {
                all_done = false;
            }
        }
        ReleaseMutex(pi_mutex);
        
        if(all_done) break;
        Sleep(1);
    }
    
    // Signal threads to exit
    WaitForSingleObject(pi_mutex, INFINITE);
    next_block = N;
    for(int i = 0; i < thread_count; ++i) {
        if(is_working[i]) continue;
        thread_data[i].start = N;
        ResumeThread(thread_pool[i]);
    }
    ReleaseMutex(pi_mutex);
    
    WaitForMultipleObjects(thread_count, &thread_pool[0], TRUE, INFINITE);
    QueryPerformanceCounter(&end);
    
    // Cleanup
    for(int i = 0; i < thread_count; ++i) {
        CloseHandle(thread_pool[i]);
    }
    CloseHandle(pi_mutex);
    
    double elapsed = (end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
    
    cout << fixed << setprecision(15);
    cout << "Pi = " << pi_result / N << endl;
    cout << "Time: " << elapsed << " ms" << endl;
    cin.get();
    cin.get();

    return 0;
}