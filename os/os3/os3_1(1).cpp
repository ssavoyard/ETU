#include <windows.h>
#include <iostream>
#include <iomanip>

using namespace std;

const long TOTAL = 100000000;      // Общее количество шагов
const long BLOCK = 3311200;        // Размер порции для одного потока

double pi_sum = 0.0;               // Накопленная сумма
HANDLE lock;                       // Защищает pi_sum и флаги

int num_threads;                   // Количество потоков (вводится пользователем)
bool* thread_busy;                 // Занятость потоков
HANDLE* thread_handles;            // Массив потоков
long current_pos = 0;              // Текущая позиция для обработки

struct Task {
    int tid;          // Номер потока
    long begin;       // Начало диапазона
    long end;         // Конец диапазона
};

Task* tasks;         // Массив заданий для потоков

DWORD WINAPI compute_pi(LPVOID arg) {
    Task* my_task = (Task*)arg;

    while (true) {
        double sum = 0.0;
        double step = 1.0 / TOTAL;

        for (long i = my_task->begin; i < my_task->end && i < TOTAL; ++i) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }

        WaitForSingleObject(lock, INFINITE);
        pi_sum += sum;
        thread_busy[my_task->tid] = false;
        ReleaseMutex(lock);

        SuspendThread(GetCurrentThread());

        if (my_task->begin >= TOTAL) break;
    }

    return 0;
}

int main() {
    cout << "Enter threads number: ";
    cin >> num_threads;

    lock = CreateMutex(NULL, FALSE, NULL);
    thread_busy = new bool[num_threads]();
    thread_handles = new HANDLE[num_threads];
    tasks = new Task[num_threads];

    // Начальное распределение работы
    for (int i = 0; i < num_threads; ++i) {
        long start = current_pos;
        current_pos += BLOCK;
        long finish = current_pos;

        tasks[i] = {i, start, finish};
        thread_busy[i] = true;
        thread_handles[i] = CreateThread(NULL, 0, compute_pi, &tasks[i], CREATE_SUSPENDED, NULL);
    }

    LARGE_INTEGER freq, t1, t2;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t1);

    // Запуск всех потоков
    for (int i = 0; i < num_threads; ++i) {
        ResumeThread(thread_handles[i]);
    }

    // Главный цикл распределения работы
    bool flag = true;
    while (flag) {
        flag = false;

        WaitForSingleObject(lock, INFINITE);
        for (int i = 0; i < num_threads; ++i) {
            if (!thread_busy[i] && current_pos < TOTAL) {
                long start = current_pos;
                long finish = start + BLOCK;
                current_pos += BLOCK;

                tasks[i] = {i, start, finish};
                thread_busy[i] = true;
                ResumeThread(thread_handles[i]);
            }

            if (thread_busy[i] || current_pos < TOTAL) {
                flag = true;
            }
        }
        ReleaseMutex(lock);

        Sleep(1);
    }

    QueryPerformanceCounter(&t2);
    double time_taken = (double)(t2.QuadPart - t1.QuadPart) / freq.QuadPart * 1000;

    // Завершение работы потоков
    for (int i = 0; i < num_threads; ++i) {
        tasks[i].begin = TOTAL;
        ResumeThread(thread_handles[i]);
    }
    WaitForMultipleObjects(num_threads, thread_handles, TRUE, INFINITE);

    // Освобождение ресурсов
    for (int i = 0; i < num_threads; ++i) {
        CloseHandle(thread_handles[i]);
    }
    CloseHandle(lock);
    delete[] thread_busy;
    delete[] thread_handles;
    delete[] tasks;

    // Вывод результатов
    cout << fixed << setprecision(15);
    cout << "Pi approximation: " << pi_sum / TOTAL << endl;
    cout << "Time: " << time_taken << " ms" << endl;

    return 0;
}