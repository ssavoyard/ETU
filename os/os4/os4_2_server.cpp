// // server.cpp
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const string PIPE_NAME = "\\\\.\\pipe\\ChatPipe";
const long BUFFER_SIZE = 512;
const long MAX_HISTORY = 1024;

HANDLE pipe_handle = NULL;
bool client_connected = false;
vector<string> message_history;

void AddToHistory(const string& tag, const string& message) {
    if (message_history.size() >= MAX_HISTORY) return;
    message_history.push_back(tag + " " + message);
}

int main() {
    int choice;
    while (true) {
        cout << "Message history:" << endl;
        for (const auto& message : message_history) {
            cout << message << endl;
        }
        cout << endl << endl;
        
        cout << "Choose option for server:" << endl;
        cout << "1. Create pipe" << endl;
        cout << "2. Wait for client" << endl;
        cout << "3. Send message" << endl;
        cout << "4. Check messages" << endl;
        cout << "5. Disconnect" << endl;
        cout << "6. Exit" << endl;
        cin >> choice;
        cin.get();
        
        switch (choice) {
            case 1: {
                if (pipe_handle) CloseHandle(pipe_handle);
                pipe_handle = CreateNamedPipe(PIPE_NAME.c_str(), PIPE_ACCESS_DUPLEX,
                                            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
                                            1, BUFFER_SIZE, BUFFER_SIZE, 0, NULL);
                cout << (pipe_handle == INVALID_HANDLE_VALUE ? "CreateNamedPipe error" : "Pipe created.") << endl;
                break;
            }
            
            case 2: {
                if (!pipe_handle) {
                    cout << "Create pipe first." << endl;
                    break;
                }
                cout << "Waiting client..." << endl;
                if (ConnectNamedPipe(pipe_handle, NULL) || GetLastError() == ERROR_PIPE_CONNECTED) {
                    client_connected = true;
                    cout << "Client connected." << endl;
                } else {
                    cout << "Connect error." << endl;
                }
                break;
            }
            
            case 3: {
                if (!client_connected) {
                    cout << "No client." << endl;
                    break;
                }
                cout << "Message: ";
                string message;
                getline(cin, message);
                DWORD bytes_written;
                if (WriteFile(pipe_handle, message.c_str(), 
                             static_cast<DWORD>(message.size() + 1), &bytes_written, NULL)) {
                    AddToHistory("[SERVER]", message);
                } else {
                    cout << "Write error." << endl;
                }
                break;
            }
            
            case 4: {
                if (!client_connected) {
                    cout << "No client." << endl;
                    break;
                }
                char buffer[BUFFER_SIZE] = {0};
                DWORD bytes_read = 0;
                if (ReadFile(pipe_handle, buffer, BUFFER_SIZE, &bytes_read, NULL)) {
                    cout << "[CLIENT] " << buffer << endl;
                    AddToHistory("[CLIENT]", buffer);
                }
                break;
            }
            
            case 5: {
                if (pipe_handle) {
                    DisconnectNamedPipe(pipe_handle);
                    client_connected = false;
                    cout << "Disconnected." << endl;
                }
                break;
            }
            
            case 6:
                if (pipe_handle) {
                    CloseHandle(pipe_handle);
                }
                return 0;
                
            default:
                cout << "Invalid choice!" << endl;
        }
    }
}