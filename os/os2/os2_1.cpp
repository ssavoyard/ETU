#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

int main(){
    int choice;
    while (true){
        cout << "1. Get system info" << endl;
        cout << "2. Get memory status" << endl;
        cout << "3. Get memory region information" << endl;
        cout << "4. Allocate memory separately" << endl;
        cout << "5. Allocate memory simultaniously" << endl;
        cout << "6. Enter data in specific memory cells" << endl;
        cout << "7. Set access levels" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch(choice) {
            case 1:
                {   
                    SYSTEM_INFO sinfo;
                    GetSystemInfo(&sinfo);
                    cout << "System info:" << endl;
                    cout << "Page size: " << sinfo.dwPageSize << endl;
                    cout << "Minimum allocation address: " << sinfo.lpMinimumApplicationAddress << endl;
                    cout << "Maximum allocation address: " << sinfo.lpMaximumApplicationAddress << endl;
                    cout << "Active processor mask: " << sinfo.dwActiveProcessorMask << endl;
                    cout << "Number of processors: " << sinfo.dwNumberOfProcessors << endl;
                    cout << "Processor type: " << sinfo.dwProcessorType << endl;
                    cout << "Allocation granularity: " << sinfo.dwAllocationGranularity << endl;
                    cout << "Processor level: " << sinfo.wProcessorLevel << endl;
                    cout << "Processor revision: " << sinfo.wProcessorRevision << endl;
                }
                break;
            case 2:
                {
                    MEMORYSTATUS temp;
                    GlobalMemoryStatus(&temp);
                
                    cout << "Memory status:" << endl;
                    cout << "Memory Load: " << temp.dwMemoryLoad << "%" << endl;
                    cout << "Total Physical: " << temp.dwTotalPhys << " bytes" << endl;
                    cout << "Available Physical: " << temp.dwAvailPhys << " bytes" << endl;
                    cout << "Total Page File: " << temp.dwTotalPageFile << " bytes" << endl;
                    cout << "Available Page File: " << temp.dwAvailPageFile << " bytes" << endl;
                    cout << "Total Virtual: " << temp.dwTotalVirtual << " bytes" << endl;
                    cout << "Available Virtual: " << temp.dwAvailVirtual << " bytes" << endl;
                }
                break;
                case 3:
                {
                    cout << "Enter address in hex format: ";
                    LPVOID address;
                    cin >> hex >> address;
                
                    MEMORY_BASIC_INFORMATION mbi;
                
                    if (VirtualQuery(address, &mbi, sizeof(mbi)) == 0) {
                        cout << "Information acquisition error."<< endl;
                        break;
                    }
                
                    cout << "Memory region information:" << endl;
                    cout << "BaseAddress " << mbi.BaseAddress << endl;
                    cout << "AllocationBase: " << mbi.AllocationBase << endl;
                    cout << "AllocationProtect: " << hex << mbi.AllocationProtect << endl;
                    cout << "RegionSize: " << dec << mbi.RegionSize << " bytes" << endl;;
                    cout << "State: " << mbi.State << " (";
                    if (mbi.State == MEM_COMMIT) cout << "MEM_COMMIT";
                    else if (mbi.State == MEM_RESERVE) cout << "MEM_RESERVE";
                    else if (mbi.State == MEM_FREE) cout << "MEM_FREE";
                    cout << ")" << endl;
                
                    cout << "Protect: " << hex << mbi.Protect << " (";
                    if (mbi.Protect & PAGE_READONLY) cout << "PAGE_READONLY ";
                    if (mbi.Protect & PAGE_READWRITE) cout << "PAGE_READWRITE ";
                    if (mbi.Protect & PAGE_EXECUTE) cout << "PAGE_EXECUTE ";
                    if (mbi.Protect & PAGE_EXECUTE_READ) cout << "PAGE_EXECUTE_READ ";
                    if (mbi.Protect & PAGE_EXECUTE_READWRITE) cout << "PAGE_EXECUTE_READWRITE ";
                    if (mbi.Protect & PAGE_NOACCESS) cout << "PAGE_NOACCESS";
                    cout << ")" << endl;
                
                    cout << "Type: " << mbi.Type << endl;
                }
                break;
                case 4:
                {
                    int mode;
                    cout << "Choose an option." << endl;
                    cout << "1. Automatic address choice" << endl;
                    cout << "2. Manual address input" << endl;;
                    cin >> mode;
                
                    LPVOID address = nullptr;
                    SIZE_T size = 0;
                
                    if (mode == 2) {
                        cout << "Enter address in hex format: ";
                        cin >> hex >> address;
                    }
                
                    cout << "Enter region size (bytes): ";
                    cin >> size;
                
                    LPVOID allocatedMemory = VirtualAlloc(address, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
                
                    if (allocatedMemory == nullptr) {
                        cout << "Memory allocation error." << endl;
                        break;
                    }
                
                    cout << "Memory successfully allocated at " << allocatedMemory << endl;;
                
                    if (!VirtualFree(allocatedMemory, 0, MEM_RELEASE)) {
                        cout << "Memory free error." << endl;
                    } else {
                        cout << "Memory freed successfully." << endl;
                    }
                }
                break;
                case 5:
                {
                    int mode;
                    cout << "Select allocation mode:" << endl;
                    cout << "1. Automatic (system chooses address)" << endl;
                    cout << "2. Manual (specify address)" << endl;
                    cin >> mode;
                
                    LPVOID desiredAddress = nullptr;
                    SIZE_T size = 0;
                
                    if (mode == 2) {
                        cout << "Enter region start address (hex, e.g., 0x00000000): ";
                        cin >> hex >> desiredAddress;
                    }
                
                    cout << "Enter region size (in bytes): ";
                    cin >> size;
                
                    LPVOID allocatedMemory = VirtualAlloc(desiredAddress, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
                
                    if (allocatedMemory == nullptr) {
                        cout << "Memory allocation error." << endl;
                        break;
                    }
                
                    cout << "Memory successfully allocated at: " << allocatedMemory << endl;
                
                    int* testData = (int*)allocatedMemory;
                    *testData = 0xDEADBEEF;
                    cout << "Test value 0x" << hex << *testData << " written to memory" << endl;

                    int readValue = *testData;
                    if (readValue != 0xDEADBEEF) {
                        cout << "Memory verification failed! Read: 0x" << hex << readValue << endl;
                    }

                    if (!VirtualFree(allocatedMemory, 0, MEM_RELEASE)) {
                        cout << "Memory free error." << endl;
                    } else {
                        cout << "Memory freed successfully" << endl;
                    }
                }
                break;
            case 6:
                {
                    LPVOID address;
                    int value;
                
                    cout << "Enter memory address (hex): 0x";
                    cin >> hex >> address;
                    cout << "Enter integer value to write: ";
                    cin >> dec >> value;

                    MEMORY_BASIC_INFORMATION mbi;
                    if (!VirtualQuery(address, &mbi, sizeof(mbi))) {
                        cout << "Error: Cannot query memory (Error " << GetLastError() << ")" << endl;
                        break;
                    }
                
                    if (!(mbi.Protect & PAGE_READWRITE)) {
                        cout << "Error: Memory is not writable (Current protection: 0x" << hex << mbi.Protect << ")" << endl;
                        break;
                    }
                
                    *(int*)address = value;
                    cout << "Success: Wrote value " << dec << value << " to 0x" << hex << address << endl;
                }
                break;
            case 7:
                {
                    LPVOID address;
                    SIZE_T size;
                    DWORD newProtect, oldProtect;
                
                    cout << "Enter region start address (hex): 0x";
                    cin >> hex >> address;
                    cout << "Enter region size (bytes): ";
                    cin >> dec >> size;
                    cout << "Select protection:\n"
                         << "1. PAGE_READONLY\n"
                         << "2. PAGE_READWRITE\n"
                         << "3. PAGE_EXECUTE_READ\n"
                         << "4. PAGE_NOACCESS\n"
                         << "Choice: ";
                    
                    int choice;
                    cin >> choice;
                    
                    switch(choice) {
                        case 1: newProtect = PAGE_READONLY; break;
                        case 2: newProtect = PAGE_READWRITE; break;
                        case 3: newProtect = PAGE_EXECUTE_READ; break;
                        case 4: newProtect = PAGE_NOACCESS; break;
                        default:
                            cout << "Error: Invalid choice" << endl;
                            break;
                    }
                
                    if (VirtualProtect(address, size, newProtect, &oldProtect)) {
                        cout << "Error: VirtualProtect failed (Error " << GetLastError() << ")" << endl;
                        break;
                    }
                
                    cout << "Protection changed successfully" << endl;
                }
                break;
            case 0:
                return 0;
            default:
                break;
        }
    }
}