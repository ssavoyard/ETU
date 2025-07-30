#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

int main() {
    int choice;
    char currentFolder[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentFolder);

    while (true) {
        cout << "1. List drives" << endl;
        cout << "2. Get drive info" << endl;
        cout << "3. Create directory" << endl;
        cout << "4. Remove directory" << endl;
        cout << "5. Create file" << endl;
        cout << "6. Copy file" << endl;
        cout << "7. Move file" << endl;
        cout << "8. File attributes" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch(choice) {
            case 1:
                {   
                    cout << "Drives list:" << endl;
                    unsigned short drives = GetLogicalDrives();
                    for (int i = ((sizeof(unsigned short) * 8) - 1); i >= 0; i--) {
                        unsigned int mask = 1 << i;
                        if (drives & mask) cout << char('A' + i) << endl;
                    }
                }
                break;
            case 2:
                {
                    string disk;
                    cout << "Enter drive name (example - C): ";
                    cin >> disk;
                    disk = disk + ":\\";
                    UINT type = GetDriveType(disk.c_str());
                    cout << "Drive type: " << type << endl;

                    char NameBuffer[MAX_PATH];
                    char SysNameBuffer[MAX_PATH];
                    DWORD VSNumber;
                    DWORD MCLength;
                    DWORD fileSF;

                    if (GetVolumeInformation(disk.c_str(),NameBuffer, sizeof(NameBuffer),
                        &VSNumber,&MCLength,&fileSF,SysNameBuffer,sizeof(SysNameBuffer)))
                    {
                        cout << "Disk type: " << NameBuffer << endl;
                        cout << "File system: " << SysNameBuffer << endl;
                        cout << "Serial number: " << VSNumber << endl;
                    }

                    DWORD sectors;
                    DWORD bytes;
                    DWORD freeClusters;
                    DWORD totalClusters;
                    GetDiskFreeSpace(disk.c_str(), &sectors, &bytes, &freeClusters, &totalClusters);
                    cout << "Bytes per sector: " << bytes << endl;
                    cout << "Sectors per claster: " << sectors << endl;
                    cout << "Free clusters: " << freeClusters << endl;
                    cout << "Total clusters: " << totalClusters << endl;
                    
                    cout << "System glags:" << endl;
                    if (fileSF & FILE_CASE_SENSITIVE_SEARCH) cout << "FILE_CASE_SENSITIVE_SEARCH" << endl;
                    if (fileSF & FILE_CASE_PRESERVED_NAMES) cout << "FILE_CASE_PRESERVED_NAMES" << endl;
                    if (fileSF & FILE_UNICODE_ON_DISK) cout << "FILE_UNICODE_ON_DISK" << endl;
                    if (fileSF & FILE_PERSISTENT_ACLS) cout << "FILE_PERSISTENT_ACLS" << endl;
                    if (fileSF & FILE_FILE_COMPRESSION) cout << "FILE_FILE_COMPRESSION" << endl;
                    if (fileSF & FILE_VOLUME_IS_COMPRESSED) cout << "FILE_VOLUME_IS_COMPRESSED" << endl;
                    if (fileSF & FILE_SUPPORTS_ENCRYPTION) cout << "FILE_SUPPORTS_ENCRYPTION" << endl;
                    if (fileSF & FILE_SUPPORTS_SPARSE_FILES) cout << "FILE_SUPPORTS_SPARSE_FILES" << endl;
                    if (fileSF & FILE_SUPPORTS_REPARSE_POINTS) cout << "FILE_SUPPORTS_REPARSE_POINTS" << endl;
                    if (fileSF & FILE_SUPPORTS_HARD_LINKS) cout << "FILE_SUPPORTS_HARD_LINKS" << endl;
                    if (fileSF & FILE_SUPPORTS_EXTENDED_ATTRIBUTES) cout << "FILE_SUPPORTS_EXTENDED_ATTRIBUTES" << endl;
                    if (fileSF & FILE_SUPPORTS_TRANSACTIONS) cout << "FILE_SUPPORTS_TRANSACTIONS" << endl;
                }
                break;
            case 3:
                {
                    string userPath;
                    cout << "Enter the path to create the folder: ";
                    cin >> userPath;
                
                    if (CreateDirectory(userPath.c_str(), NULL)) {
                        cout << "Folder successfully created." << endl;
                    } else {
                        cout << "Error creating the folder. The folder already exists or the path is invalid." << endl;
                    }
                }
                break;
            case 4:
                {
                    string userPath;
                    cout << "Enter the folder path to delete: ";
                    cin >> userPath;
                
                    if (RemoveDirectory(userPath.c_str())) {
                        cout << "Folder successfully deleted." << endl;
                    } else {
                        cout << "Error deleting the folder. The does not exist, is empty, or path is invalid." << endl;
                    }
                }
                break;
            case 5:
                {
                    string userPath;
                    cout << "Enter the file path to create: ";
                    cin >> userPath;
                
                    HANDLE hf = CreateFile(userPath.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
                
                    if (hf != INVALID_HANDLE_VALUE) {
                        cout << "File created successfully." << endl;
                        CloseHandle(hf);
                    } else {
                        cout << "Error: Failed to create file." << endl;
                    }
                }
                break;
            case 6:
                {
                    string sourcePath, destPath;
                    cout << "Enter the source file path: ";
                    cin >> sourcePath;
                    cout << "Enter the destination file path: ";
                    cin >> destPath;
                
                    if (CopyFile(sourcePath.c_str(), destPath.c_str(), TRUE)) {
                        cout << "File copied successfully." << endl;
                    } else {
                        cout << "Error: Failed to copy file." << endl;
                    }
                }
                break;
            case 7:
                {
                    string sourcePath, destPath;
                    cout << "Enter the source file path: ";
                    cin >> sourcePath;
                    cout << "Enter the destination file path: ";
                    cin >> destPath;
                
                    if (MoveFile(sourcePath.c_str(), destPath.c_str())) {
                        cout << "File moved successfully." << endl;
                    } else {
                        cout << "Error: Failed to move file." << endl;
                    }
                }
                break;
            case 8:
                // {
                //     string pathStr = string(currentFolder) + "\\ATTRIBUTE_TEST.txt";
                //     DWORD attrs = GetFileAttributes(pathStr.c_str());
                
                //     cout << "Attributes of docx file: " << endl;
                //     if (attrs & FILE_ATTRIBUTE_READONLY) cout << "READONLY" << endl;
                //     if (attrs & FILE_ATTRIBUTE_HIDDEN) cout << "HIDDEN" << endl;
                //     if (attrs & FILE_ATTRIBUTE_SYSTEM) cout << "SYSTEM" << endl;
                //     if (attrs & FILE_ATTRIBUTE_DIRECTORY) cout << "DIRECTORY" << endl;
                //     if (attrs & FILE_ATTRIBUTE_ARCHIVE) cout << "ARCHIVE" << endl;
                //     if (attrs & FILE_ATTRIBUTE_ENCRYPTED) cout << "ENCRYPTED" << endl;
                //     if (attrs & FILE_ATTRIBUTE_COMPRESSED)cout << "COMPRESSED" << endl;
                //     if (attrs & FILE_ATTRIBUTE_OFFLINE) cout << "OFFLINE" << endl;
                //     if (attrs & FILE_ATTRIBUTE_TEMPORARY) cout << "TEMPORARY" << endl;

                //     attrs |= FILE_ATTRIBUTE_HIDDEN;
                //     SetFileAttributes(pathStr.c_str(), attrs);

                //     cout << "HIDDEN attribute set" << endl;

                //     cout << "Press any key to continue..." << endl;
                //     cin.get();
                //     cin.get();

                //     attrs &= ~FILE_ATTRIBUTE_HIDDEN;
                //     SetFileAttributes(pathStr.c_str(), attrs);
                //     cout << "HIDDEN attribute removed" << endl;

                //     HANDLE hf = CreateFile(pathStr.c_str(),GENERIC_READ,FILE_SHARE_READ,NULL,
                //         OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
                
                //     BY_HANDLE_FILE_INFORMATION fileInfo;
                //     if (GetFileInformationByHandle(hf, &fileInfo)) {
                //         cout << "File information retrieved successfully.";
                //         CloseHandle(hf);
                //     }
                
                //     FILETIME creationTime = fileInfo.ftCreationTime;
                //     FILETIME lastAccessTime = fileInfo.ftLastAccessTime;
                //     FILETIME lastWriteTime = fileInfo.ftLastWriteTime;
                
                //     SYSTEMTIME stCreate = { 0 }, stAccess = { 0 }, stWrite = { 0 };
                //     FileTimeToSystemTime(&creationTime,  &stCreate);
                //     FileTimeToSystemTime(&lastAccessTime,&stAccess);
                //     FileTimeToSystemTime(&lastWriteTime, &stWrite);
                
                //     cout << "Creation Time: " << stCreate.wDay << "." << stCreate.wMonth << "." << stCreate.wYear
                //               << " " << stCreate.wHour + 3 << ":" << stCreate.wMinute << ":" << stCreate.wSecond << endl;
                
                //     cout << "Last Access Time: " << stAccess.wDay << "." << stAccess.wMonth   << "." << stAccess.wYear
                //               << " " << stAccess.wHour + 3 << ":" << stAccess.wMinute << ":" << stAccess.wSecond << endl;
                
                //     cout << "Last Write Time: " << stWrite.wDay  << "." << stWrite.wMonth  << "." << stWrite.wYear
                //             << " " << stWrite.wHour + 3 << ":" << stWrite.wMinute  << ":" << stWrite.wSecond  << endl;
                
                //     FILETIME originalAccessTime = lastAccessTime;
                //     FILETIME originalWriteTime = lastWriteTime;
                
                //     SYSTEMTIME st;
                //     GetSystemTime(&st);
                //     FILETIME newFt;

                //     SystemTimeToFileTime(&st, &newFt);
                //     SetFileTime(hf, NULL, &newFt, &newFt);
                //     cout << "File time chamged to current." << endl;
                //     cout << "Press any key to continue..." << endl;
                //     cin.get();
                //     cin.get();
                //     SetFileTime(hf, NULL, &originalAccessTime, &originalWriteTime);
                //     cout << "Original time restored." << endl;

                //     CloseHandle(hf);
                // }
                {
                    string filePath;
                    cout << "Enter the file path: ";
                    cin >> filePath;

                    DWORD attrs = GetFileAttributes(filePath.c_str());
                    if (attrs == INVALID_FILE_ATTRIBUTES) {
                        cout << "Error: Failed to get file attributes." << endl;
                        break;
                    }

                    cout << "Attributes of the file: " << endl;
                    if (attrs & FILE_ATTRIBUTE_READONLY) cout << "READONLY" << endl;
                    if (attrs & FILE_ATTRIBUTE_HIDDEN) cout << "HIDDEN" << endl;
                    if (attrs & FILE_ATTRIBUTE_SYSTEM) cout << "SYSTEM" << endl;
                    if (attrs & FILE_ATTRIBUTE_DIRECTORY) cout << "DIRECTORY" << endl;
                    if (attrs & FILE_ATTRIBUTE_ARCHIVE) cout << "ARCHIVE" << endl;
                    if (attrs & FILE_ATTRIBUTE_ENCRYPTED) cout << "ENCRYPTED" << endl;
                    if (attrs & FILE_ATTRIBUTE_COMPRESSED) cout << "COMPRESSED" << endl;
                    if (attrs & FILE_ATTRIBUTE_OFFLINE) cout << "OFFLINE" << endl;
                    if (attrs & FILE_ATTRIBUTE_TEMPORARY) cout << "TEMPORARY" << endl;

                    attrs |= FILE_ATTRIBUTE_HIDDEN;
                    if (SetFileAttributes(filePath.c_str(), attrs)) {
                        cout << "HIDDEN attribute set." << endl;
                    } else {
                        cout << "Error: Failed to set HIDDEN attribute." << endl;
                    }

                    cout << "Press any key to continue..." << endl;
                    cin.get();
                    cin.get();

                    attrs &= ~FILE_ATTRIBUTE_HIDDEN;
                    if (SetFileAttributes(filePath.c_str(), attrs)) {
                        cout << "HIDDEN attribute removed." << endl;
                    } else {
                        cout << "Error: Failed to remove HIDDEN attribute." << endl;
                    }

                    HANDLE hf = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                    if (hf == INVALID_HANDLE_VALUE) {
                        cout << "Error: Failed to open file." << endl;
                        break;
                    }

                    BY_HANDLE_FILE_INFORMATION fileInfo;
                    if (GetFileInformationByHandle(hf, &fileInfo)) {
                        cout << "File information retrieved successfully." << endl;
                    } else {
                        cout << "Error: Failed to retrieve file information." << endl;
                        CloseHandle(hf);
                        break;
                    }

                    FILETIME creationTime = fileInfo.ftCreationTime;
                    FILETIME lastAccessTime = fileInfo.ftLastAccessTime;
                    FILETIME lastWriteTime = fileInfo.ftLastWriteTime;

                    SYSTEMTIME stCreate = { 0 }, stAccess = { 0 }, stWrite = { 0 };
                    FileTimeToSystemTime(&creationTime, &stCreate);
                    FileTimeToSystemTime(&lastAccessTime, &stAccess);
                    FileTimeToSystemTime(&lastWriteTime, &stWrite);

                    cout << "Creation Time: " << stCreate.wDay << "." << stCreate.wMonth << "." << stCreate.wYear
                        << " " << stCreate.wHour + 3 << ":" << stCreate.wMinute << ":" << stCreate.wSecond << endl;

                    cout << "Last Access Time: " << stAccess.wDay << "." << stAccess.wMonth << "." << stAccess.wYear
                        << " " << stAccess.wHour + 3 << ":" << stAccess.wMinute << ":" << stAccess.wSecond << endl;

                    cout << "Last Write Time: " << stWrite.wDay << "." << stWrite.wMonth << "." << stWrite.wYear
                        << " " << stWrite.wHour + 3 << ":" << stWrite.wMinute << ":" << stWrite.wSecond << endl;

                    FILETIME originalAccessTime = lastAccessTime;
                    FILETIME originalWriteTime = lastWriteTime;

                    SYSTEMTIME st;
                    GetSystemTime(&st);
                    FILETIME newFt;
                    SystemTimeToFileTime(&st, &newFt);

                    if (!SetFileTime(hf, NULL, &newFt, &newFt)) {
                        cout << "File time changed to current." << endl;
                    } else {
                        cout << "Error: Failed to change file time." << endl;
                    }

                    cout << "Press any key to continue..." << endl;
                    cin.get();

                    if (!SetFileTime(hf, NULL, &originalAccessTime, &originalWriteTime)) {
                        cout << "Original time restored." << endl;
                    } else {
                        cout << "Error: Failed to restore original time." << endl;
                    }

                    CloseHandle(hf);
                }
                break;
            case 0:
                return 0;
            default:
                break;
        }
    }
    return 0;
}