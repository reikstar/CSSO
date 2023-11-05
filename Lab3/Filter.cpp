#include "utils.h"
#include <tlhelp32.h>

void countLinesWithString(const string &filePath, const string &stringToSearch){

    HANDLE hFile = CreateFile(filePath.c_str(), 
    GENERIC_READ,
    FILE_SHARE_READ,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL
    );
    
    if(hFile == INVALID_HANDLE_VALUE){
        cerr << "Error opening file: " << filePath << " (" << GetLastError() << ")" << endl;
        return;
    }
    
    const DWORD BUFF_SIZE = 0x8000;
    DWORD bytesRead;
    char buffer[BUFF_SIZE];
    memset(buffer, '\0', BUFF_SIZE);

    bool successfulRead = ReadFile(hFile, buffer, BUFF_SIZE-1, &bytesRead, NULL) != 0;

    if(!successfulRead){
        cout << "Error reading file: " << filePath << " (" << GetLastError() << ")" << endl;
        return;
    }
    
    CloseHandle(hFile);
    if(bytesRead > 0 && bytesRead <= BUFF_SIZE-1){
        buffer[bytesRead] = '\0';

    }
    else{
        if(bytesRead > BUFF_SIZE-1){
            buffer[bytesRead] = '\0';
        }
               
    }
    string fileContent(buffer, bytesRead);
    int count = 0;
    size_t pos = 0, prevPos = 0;
    while((pos = fileContent.find('\n', prevPos)) != string::npos){
        string line = fileContent.substr(prevPos, pos - prevPos);

        if(line.find(stringToSearch) != string::npos){
            
            count++;
        }
        prevPos = pos + 1;
        
    }
    string fileName;
    pos = filePath.find_last_of('\\');
    fileName = filePath.substr(pos + 1);
    
    cout << "File name: " << fileName << endl << "The number of lines containing the string: " << count << endl;
}

DWORD WINAPI ThreadFunction(LPVOID lpParam){
    auto p = static_cast<pair<string, string>*>(lpParam);
    countLinesWithString(p->first, p->second);
    delete p;
    return 0; // return 0 for success
}


int main(){
    LUID luid;
    TOKEN_PRIVILEGES tp;
    HANDLE hToken;
    HANDLE hThread1;
    HANDLE hThread2;
    HANDLE hThread3;

    if (!LookupPrivilegeValue(NULL, SE_SYSTEM_PROFILE_NAME, &luid)){
        cerr << "LookupPrivilegeValue error " << GetLastError();
        return (-1);
    }

    if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)){
        cerr << "OpenProcessToken error " << GetLastError();
        return (-1);
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if(!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL)){
        cerr << "AdjustTokenPrivileges error " << GetLastError();
        CloseHandle(hToken);
        return (-1);
    }

    auto param1 = new pair<string, string>("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\procese.txt", "svchost.exe");

    auto param2 = new pair<string, string>("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\fire.txt", "OwnerProcessId = 0");
    
    auto param3 = new pair<string, string>("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\module_process.txt", "KERNEL32.DLL");

    hThread1 = CreateThread(NULL, 0, ThreadFunction, param1, 0, NULL);
    Sleep(10);
    hThread2 = CreateThread(NULL, 0, ThreadFunction, param2, 0, NULL);
    Sleep(10);
    hThread3 = CreateThread(NULL, 0, ThreadFunction, param3, 0, NULL);

    if(!hThread1 || !hThread2 || !hThread3){

        cerr << "CreateThread error " << GetLastError();
        return (-1);
    }

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);
    WaitForSingleObject(hThread3, INFINITE);

    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(hThread3);

    delete param1;
    delete param2;
    delete param2;

    cout << "Filter.exe: I'm taking a 30 min nap now.... Drifting off....\n";
    cout << "-ZZZZZZ-   -ZZZZZZ-   -ZZZZZZ- \n";
    cout << "      -Z         -Z         -Z \n";
    cout << "     -Z         -Z         -Z \n";
    cout << "    -Z         -Z         -Z \n";
    cout << "   -Z         -Z         -Z \n";
    cout << "  -Z         -Z         -Z .\n";
    cout << " -Z         -Z         -Z \n";
    cout << "-ZZZZZZZ-   -ZZZZZZZ-   -ZZZZZZZ-  . . . . . . . . . . . . . . . \n";

    Sleep(30 * 60 * 1000);
    return 0;


    
}
