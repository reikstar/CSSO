#include "utils.h"
#include <tlhelp32.h>

int main(){
    #define BUF_SIZE 256
    HANDLE hFileMapping;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    LPVOID pMap;
    char collectCmd[] = "Collect.exe";
    char filterCmd[] = "Filter.exe";
    char killerCmd[] = "Killer.exe";


    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    hFileMapping = CreateFileMapping(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        BUF_SIZE,
        "cssoh3basicsync"
    );

    if(!hFileMapping){
        cerr << "CreateFileMapping error " << GetLastError();
        return (-1);
    }

    pMap = MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, 256);
        if(pMap) {
            memset(pMap, 0, 256);
            UnmapViewOfFile(pMap);
        }

    if( !CreateProcess( NULL,
        collectCmd,
        NULL,      
        NULL,          
        FALSE,          
        0,              
        NULL,           
        NULL,           
        &si,            
        &pi )           
    ) 
    {
        cerr << "CreateProcess of Collect.exe error " << GetLastError();
        CloseHandle(hFileMapping);

        return (-1);
    }

    WaitForSingleObject( pi.hProcess, INFINITE );
    

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    pMap = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 256);
    if(!pMap){
        cerr << "MapViewOfFile error " << GetLastError();
        CloseHandle(hFileMapping);
        return (-1);
    }

    cout << "Content of File Mapping:" << endl << (char*)pMap << endl;

    if( !CreateProcess( NULL,
        filterCmd,
        NULL,      
        NULL,          
        FALSE,          
        0,              
        NULL,           
        NULL,           
        &si,            
        &pi )           
    ) 
    {
        cerr << "CreateProcess of Filter.exe error " << GetLastError();
        CloseHandle(hFileMapping);

        return (-1);
    }

    Sleep(150);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    if( !CreateProcess( NULL,
        killerCmd,
        NULL,      
        NULL,          
        FALSE,          
        0,              
        NULL,           
        NULL,           
        &si,            
        &pi )           
    ) 
    {
        cerr << "CreateProcess of Killer.exe error " << GetLastError();
        CloseHandle(hFileMapping);

        return (-1);
    }
    Sleep(10);
    for(int i = 0; i < 3; i++){
        cout<< 3 - i << "..." << endl;
        Sleep(1000);
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hFileMapping);
    UnmapViewOfFile(pMap);

    return 0;

}