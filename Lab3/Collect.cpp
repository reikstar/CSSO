#include <iostream>
#include "utils.h"
#include <tlhelp32.h>


int main(){
    HANDLE hSnapshot;
    HANDLE hModuleSnapshot;
    HANDLE hFileMap;
    DWORD CurrentExe_PID = GetCurrentProcessId();
    LPVOID pMap;
    PROCESSENTRY32 pe32;
    THREADENTRY32 te32;
    MODULEENTRY32 me32;
    int processCount = 0;
    int threadCount = 0;
    int moduleCount = 0;


    createDirectoryRecursively("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo");
    createFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\procese.txt", FILE_APPEND_DATA);
    createFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\fire.txt", FILE_APPEND_DATA);
    createFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\module_process.txt", FILE_APPEND_DATA);


    //initialize dwSize, otherwise *32First function will fail.
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPTHREAD, 0);
    if(hSnapshot == INVALID_HANDLE_VALUE){

        cerr << "CreateToolhelp32Snapshot error " << GetLastError();
        CloseHandle(hSnapshot);
        return (-1);
    }


    pe32.dwSize = sizeof(PROCESSENTRY32);
    te32.dwSize = sizeof(THREADENTRY32);
    me32.dwSize = sizeof(MODULEENTRY32);


    if(!Process32First(hSnapshot, &pe32)){
        
        cerr << "Process32First error " << GetLastError();
        CloseHandle(hSnapshot);
        return (-1);
    }

    do{ 
        
        string contentToWrite = "ParentProcessId = " + to_string(pe32.th32ParentProcessID) 
                                + ", ProcessId = " + to_string(pe32.th32ProcessID) 
                                + ", SzExeFile = " + pe32.szExeFile + "\n";
        appendToFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\procese.txt", contentToWrite);
        processCount++;

    }while(Process32Next(hSnapshot, &pe32));


    if(!Thread32First(hSnapshot, &te32)){
        
        cerr << "Thread32First error " << GetLastError();
        CloseHandle(hSnapshot);
        return (-1);
    }

    do{ 
        
        string contentToWrite = "ThreadId = " + to_string(te32.th32ThreadID) 
                                + ", OwnerProcessId = " + to_string(te32.th32OwnerProcessID) + "\n";
                                
        appendToFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\fire.txt", contentToWrite);
        threadCount++;

    }while(Thread32Next(hSnapshot, &te32));

    CloseHandle(hSnapshot);

    
    hModuleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, CurrentExe_PID);

    if(hModuleSnapshot == INVALID_HANDLE_VALUE){
        cerr << "CreateToolhelp32Snapshot Module error " << GetLastError();
        CloseHandle(hModuleSnapshot);
        return (-1);
    }

    if(!Module32First(hModuleSnapshot, &me32)){
        cerr << "Module32First error " << GetLastError();
        CloseHandle(hModuleSnapshot);
        return (-1);
    }

    do{ 

        string contentToWrite = "ModuleId = " + to_string(me32.th32ModuleID) 
                                + ", ProcessId = " + to_string(me32.th32ProcessID)
                                + ", SzModule = " + me32.szModule
                                + ", SzExePath = " + me32.szExePath + "\n";
        appendToFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\ProcessInfo\\module_process.txt", contentToWrite);
        moduleCount++;
    } while(Module32Next(hModuleSnapshot, &me32));

    CloseHandle(hModuleSnapshot);

    hFileMap = OpenFileMapping(
                    FILE_MAP_ALL_ACCESS,
                    FALSE,
                    "cssoh3basicsync"
    );

    string fileMappingBufferToWrite = "1. Module:" + to_string(moduleCount) + "\n2. Procese:" + to_string(processCount) + "\n3. Fire:" +  to_string(threadCount) + "\n";

    if(hFileMap == NULL){
        cerr << "Open file mapping object error " << GetLastError();
        CloseHandle(hFileMap);
        return (-1); 
    }

     pMap =  (LPSTR) MapViewOfFile(hFileMap, FILE_MAP_WRITE, 0, 0, 256);

     if(pMap == NULL){
        cout << "MapViewOfFile error " << GetLastError();
        CloseHandle(hFileMap);
        return (-1);
    }

    const char* source = fileMappingBufferToWrite.c_str();
    DWORD sizeToCopy = fileMappingBufferToWrite.size() + 1;

    if (sizeToCopy > 256) {
    cerr << "Data to write exceeds the shared memory size." << endl;
    UnmapViewOfFile(pMap);
    CloseHandle(hFileMap);
    return (-1);
}

    memcpy(pMap, source, sizeToCopy);

    UnmapViewOfFile(pMap);
    CloseHandle(hFileMap);


}