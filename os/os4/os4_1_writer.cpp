// writer.cpp
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
constexpr int TOTAL_SIZE = PAGE_COUNT * PAGE_SIZE;

class SharedMemoryWriter {
public:
    SharedMemoryWriter(int id) : writerId(id), gen(std::random_device{}()) {
        initLog();
        initSharedMemory();
        initPageMutexes();
    }

    ~SharedMemoryWriter() {
        if (logFile.is_open()) logFile.close();
        if (pageBase) UnmapViewOfFile(pageBase);
        for (auto& hMutex : pageMutexes) {
            if (hMutex) CloseHandle(hMutex);
        }
    }

    void run() {
        std::uniform_int_distribution<> sleepDist(500, 1500);

        for (int i = 0; i < 5; ++i) {
            int pageIndex = findCleanPage();
            if (pageIndex >= 0) {
                performWriteIteration(pageIndex, sleepDist(gen));
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
        std::cout << "Writer " << writerId << " finished\n";
    }

private:
    int writerId;
    std::ofstream logFile;
    HANDLE hMap = nullptr;
    BYTE* pageBase = nullptr;
    std::vector<HANDLE> pageMutexes;
    std::mt19937 gen;

    void initLog() {
        std::string filename = "writers_logs/writer_" + std::to_string(writerId) + ".log";
        logFile.open(filename);
        if (!logFile) {
            throw std::runtime_error("Failed to open log file");
        }
    }

    void initSharedMemory() {
        hMap = CreateFileMappingW(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            TOTAL_SIZE,
            SHM_NAME
        );

        if (!hMap) throw std::runtime_error("CreateFileMapping failed");

        pageBase = static_cast<BYTE*>(MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0));
        if (!pageBase) throw std::runtime_error("MapViewOfFile failed");

        for (int i = 0; i < PAGE_COUNT; ++i) {
            *reinterpret_cast<char*>(pageBase + i * PAGE_SIZE) = '0';
        }
    }

    void initPageMutexes() {
        for (int i = 0; i < PAGE_COUNT; ++i) {
            std::wstring mutexName = PAGE_MUTEX_NAME + std::to_wstring(i);
            HANDLE hMutex = CreateMutexW(NULL, FALSE, mutexName.c_str());
            if (!hMutex) {
                throw std::runtime_error("Failed to create page mutex");
            }
            pageMutexes.push_back(hMutex);
        }
    }

    int findCleanPage() {
        std::uniform_int_distribution<int> dist(0, PAGE_COUNT - 1);
        int start = dist(gen);
        for (int k = 0; k < PAGE_COUNT; ++k) {
            int i = (start + k) % PAGE_COUNT;
            
            DWORD waitResult = WaitForSingleObject(pageMutexes[i], 0);
            if (waitResult == WAIT_OBJECT_0) {
                if (*reinterpret_cast<char*>(pageBase + i * PAGE_SIZE) == '0') {
                    return i;
                }
                ReleaseMutex(pageMutexes[i]);
            }
        }
        return -1;
    }

    void logEvent(const std::string& state, int pageIndex = -1) {
        auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();

        if (pageIndex >= 0) {
            logFile << "WRITER_" << std::setw(2) << std::setfill('0') << writerId 
                   << " " << state << " PAGE_" << pageIndex << " " << timestamp << "\n";
        } else {
            logFile << "WRITER_" << std::setw(2) << std::setfill('0') << writerId 
                   << " " << state << " " << timestamp << "\n";
        }
        logFile.flush();
    }

    void performWriteIteration(int pageIndex, int durationMs) {
        logEvent("WRITE", pageIndex);
        
        *reinterpret_cast<char*>(pageBase + pageIndex * PAGE_SIZE) = '1';
        char content[16] = {0};
        sprintf(content, "hello_%02d", writerId);
        memcpy(pageBase + pageIndex * PAGE_SIZE + 1, content, strlen(content) + 1);

        std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
        
        logEvent("RELEASE", pageIndex);
        ReleaseMutex(pageMutexes[pageIndex]);
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: Writer.exe <writer_id>\n";
        return 1;
    }

    try {
        SharedMemoryWriter writer(std::stoi(argv[1]));
        writer.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}