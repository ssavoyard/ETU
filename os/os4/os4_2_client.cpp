// // client.cpp
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const string PIPE_NAME = "\\\\.\\pipe\\ChatPipe";
const long BUFFER_SIZE = 512;
const long MAX_HISTORY = 1024;

HANDLE pipe_handle = INVALID_HANDLE_VALUE;
vector<string> message_history;

struct ReadContext {
    OVERLAPPED overlapped;
    char buffer[BUFFER_SIZE];
};

void AddToHistory(const string& tag, const string& message) {
    if (message_history.size() >= MAX_HISTORY) return;
    message_history.push_back(tag + " " + message);
}

VOID CALLBACK ReadCompletionCallback(DWORD error_code, DWORD bytes_transferred, LPOVERLAPPED overlapped) {
    ReadContext* context = reinterpret_cast<ReadContext*>(overlapped);
    
    if (error_code == 0 && bytes_transferred > 0) {
        context->buffer[bytes_transferred] = '\0';
        cout << "[SERVER] " << context->buffer << endl;
        AddToHistory("[SERVER]", context->buffer);
        
        // Start next async read
        ReadContext* new_context = new ReadContext();
        ZeroMemory(&new_context->overlapped, sizeof(OVERLAPPED));
        if (!ReadFileEx(pipe_handle, new_context->buffer, BUFFER_SIZE, 
                       &new_context->overlapped, ReadCompletionCallback)) {
            delete new_context;
        }
    } else {
        cout << "Read error or no data: code=" << error_code << " bytes=" << bytes_transferred << endl;
        delete context;
    }
}

int main() {
    int choice;
    while (true) {
        cout << "Message history:" << endl;
        for (const auto& message : message_history) {
            cout << message << endl;
        }
        cout << endl << endl;
        
        cout << "Choose option for client" << endl;
        cout << "1. Connect to server" << endl;
        cout << "2. Send message" << endl;
        cout << "3. Check messages (async)" << endl;
        cout << "4. Exit" << endl;
        cin >> choice;
        cin.get();
        
        switch (choice) {
            case 1: {
                if (pipe_handle != INVALID_HANDLE_VALUE) {
                    cout << "Already connected." << endl;
                    break;
                }
                pipe_handle = CreateFile(PIPE_NAME.c_str(), GENERIC_READ | GENERIC_WRITE,
                                       0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
                cout << (pipe_handle == INVALID_HANDLE_VALUE ? "Connect error." : "Connected.") << endl;
                break;
            }
            
            case 2: {
                if (pipe_handle == INVALID_HANDLE_VALUE) {
                    cout << "Not connected." << endl;
                    break;
                }
                cout << "Message: ";
                string message;
                getline(cin, message);
                DWORD bytes_written;
                if (WriteFile(pipe_handle, message.c_str(), 
                             static_cast<DWORD>(message.size() + 1), &bytes_written, NULL)) {
                    AddToHistory("[CLIENT]", message);
                } else {
                    cout << "Write error." << endl;
                }
                break;
            }
            
            case 3: {
                if (pipe_handle == INVALID_HANDLE_VALUE) {
                    cout << "Not connected." << endl;
                    break;
                }
                ReadContext* context = new ReadContext();
                ZeroMemory(&context->overlapped, sizeof(OVERLAPPED));
                if (!ReadFileEx(pipe_handle, context->buffer, BUFFER_SIZE,
                              &context->overlapped, ReadCompletionCallback)) {
                    cout << "ReadFileEx error: " << GetLastError() << endl;
                    delete context;
                } else {
                    cout << "Async read started." << endl;
                }
                break;
            }
            
            case 4:
                return 0;
                
            default:
                cout << "Invalid choice" << endl;
        }
        SleepEx(0, TRUE);
    }
}