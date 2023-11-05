#include "utils.h"
#include <tlhelp32.h>

int main(){
    HANDLE hSnapshot;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;

    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hSnapshot == INVALID_HANDLE_VALUE){

        cerr << "CreateToolhelp32Snapshot error " << GetLastError();
        CloseHandle(hSnapshot);
        return (-1);
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);
    
    if(!Process32First(hSnapshot, &pe32)){
        
        cerr << "Process32First error " << GetLastError();
        CloseHandle(hSnapshot);
        return (-1);
    }

    bool filterFound = FALSE; 

    do{ 
        if(strcmp("Filter.exe", pe32.szExeFile) == 0){

            filterFound = TRUE;
            hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);

            if(hProcess == NULL){
                CloseHandle(hSnapshot);
                cerr << "OpenProcess error " << GetLastError();
                return (-1);
            }

            if(!TerminateProcess(hProcess, 0)){
                cerr << "TerminateProcess error" << GetLastError();
                CloseHandle(hSnapshot);
                CloseHandle(hProcess);
                return (-1);
            }
            cout << "Killer.exe: Killing Filter.exe NOW \n";
            Sleep(1000 * 3);
            CloseHandle(hProcess);
            break;

        }

    }while(Process32Next(hSnapshot, &pe32));
    
    CloseHandle(hSnapshot);
    if(!filterFound){
        cout << "Filter.exe not running." << endl;
    }
    
    return 0; 

}
