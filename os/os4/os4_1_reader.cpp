// reader.cpp
#include <windows.h>
#include <iostream>
#include <fstream>
#include <random>
#include <memory>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>

constexpr wchar_t SHM_NAME[] = L"SharedBuffer";
constexpr wchar_t PAGE_MUTEX_NAME[] = L"PageMutex_";
constexpr int PAGE_COUNT = 10;
constexpr int PAGE_SIZE = 4096;

class SharedMemoryReader {
public:
    SharedMemoryReader(int id) : readerId(id), gen(std::random_device{}()) {
        initLog();
        initSharedMemory();
    }

    ~SharedMemoryReader() {
        if (logFile.is_open()) logFile.close();
        if (pageBase) UnmapViewOfFile(pageBase);
    }

    void run() {
        std::uniform_int_distribution<> sleepDist(500, 1500);
        
        for (int i = 0; i < 5; ++i) {
            int pageIndex = findDirtyPage();
            if (pageIndex >= 0) {
                performReadIteration(pageIndex, sleepDist(gen));
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
        std::cout << "Reader " << readerId << " finished\n";
    }

private:
    int readerId;
    std::ofstream logFile;
    HANDLE hMap = nullptr;
    BYTE* pageBase = nullptr;
    std::mt19937 gen;

    void initLog() {
        std::string filename = "readers_logs/reader_" + std::to_string(readerId) + ".log";
        logFile.open(filename);
        if (!logFile) {
            throw std::runtime_error("Failed to open log file");
        }
    }

    void initSharedMemory() {
        hMap = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, SHM_NAME);
        if (!hMap) throw std::runtime_error("OpenFileMapping failed");

        pageBase = static_cast<BYTE*>(MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0));
        if (!pageBase) throw std::runtime_error("MapViewOfFile failed");
    }

    int findDirtyPage() {
        std::uniform_int_distribution<int> dist(0, PAGE_COUNT - 1);
        int start = dist(gen);
        for (int k = 0; k < PAGE_COUNT; ++k) {
            int i = (start + k) % PAGE_COUNT;
            
            std::wstring mutexName = PAGE_MUTEX_NAME + std::to_wstring(i);
            HANDLE hPageMutex = OpenMutexW(SYNCHRONIZE, FALSE, mutexName.c_str());
            if (!hPageMutex) continue;

            DWORD waitResult = WaitForSingleObject(hPageMutex, 0);
            if (waitResult == WAIT_OBJECT_0) {
                if (*reinterpret_cast<char*>(pageBase + i * PAGE_SIZE) == '1') {
                    CloseHandle(hPageMutex);
                    return i;
                }
                ReleaseMutex(hPageMutex);
            }
            CloseHandle(hPageMutex);
        }
        return -1;
    }

    void logEvent(const std::string& state, int pageIndex = -1) {
        auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();

        if (pageIndex >= 0) {
            logFile << "READER_" << std::setw(2) << std::setfill('0') << readerId 
                   << " " << state << " PAGE_" << pageIndex << " " << timestamp << "\n";
        } else {
            logFile << "READER_" << std::setw(2) << std::setfill('0') << readerId 
                   << " " << state << " " << timestamp << "\n";
        }
        logFile.flush();
    }

    void performReadIteration(int pageIndex, int durationMs) {
        logEvent("READ", pageIndex);
        
        char buffer[16] = {0};
        memcpy(buffer, pageBase + pageIndex * PAGE_SIZE + 1, 9);
        logFile << "READER_" << std::setw(2) << std::setfill('0') << readerId 
               << " READ_CONTENT PAGE_" << pageIndex 
               << " \"" << buffer << "\"\n";
        logFile.flush();

        *reinterpret_cast<char*>(pageBase + pageIndex * PAGE_SIZE) = '0';

        std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
        
        logEvent("RELEASE", pageIndex);
        
        std::wstring mutexName = PAGE_MUTEX_NAME + std::to_wstring(pageIndex);
        HANDLE hPageMutex = OpenMutexW(SYNCHRONIZE, FALSE, mutexName.c_str());
        if (hPageMutex) {
            ReleaseMutex(hPageMutex);
            CloseHandle(hPageMutex);
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: Reader.exe <reader_id>\n";
        return 1;
    }

    try {
        SharedMemoryReader reader(std::stoi(argv[1]));
        reader.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}