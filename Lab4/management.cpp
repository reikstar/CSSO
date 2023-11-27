#include "utils.h"
#include "tlhelp32.h"
#define fileSize (sizeof(DWORD) * 10000)




int main(){
    STARTUPINFO si[3];
    PROCESS_INFORMATION pi[3];
    LPCSTR programs[3] = {"C:\\Users\\Asihma\\CSSO\\Lab4\\deposit.exe", "C:\\Users\\Asihma\\CSSO\\Lab4\\donation.exe", "C:\\Users\\Asihma\\CSSO\\Lab4\\sold.exe"};
    HANDLE hMapFileShelves, hMapFileValability, hMapFilePrices,hFile, hEventDepoDone, hEventSoldDone, hEventDonationDone, hMutex;
    LPVOID pMapShelves, pMapValability, pMapPrices;
    DWORD* shelvesArray;
    DWORD* valabilityArray;
    DWORD* pricesArray;

    createDirectoryRecursively("C:\\Facultate\\CSSO\\Week4\\Reports\\Summary");
    
    createFile("C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\logs.txt", FILE_APPEND_DATA);
    createFile("C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\sold.txt", GENERIC_READ | GENERIC_WRITE);
    createFile("C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\donations.txt", GENERIC_READ | GENERIC_WRITE);

    WriteToFile("C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\sold.txt", "0");
    WriteToFile("C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\donations.txt", "0");

    
    hMapFileShelves = createMapping("MarketShelves", PAGE_READWRITE, 0, fileSize);
    hMapFileValability = createMapping("MarketValability", PAGE_READWRITE, 0 , fileSize);
    hMapFilePrices = createMapping("ProductPrices", PAGE_READWRITE, 0 , fileSize);
    hEventDepoDone = CreateEvent(NULL, TRUE, FALSE, "DepoDone");
    hEventSoldDone = CreateEvent(NULL, TRUE, FALSE, "SoldDone");
    hEventDonationDone = CreateEvent(NULL, TRUE, FALSE, "DonationDone");
    hMutex = CreateMutex(
        NULL,
        FALSE,
        "mutex");
    
    if(hEventDepoDone == NULL || hEventSoldDone == NULL || hEventDonationDone == NULL){
        CloseHandle(hEventDepoDone);
        CloseHandle(hEventSoldDone);
        CloseHandle(hEventDonationDone);
        cerr << "CreateEvent error " << GetLastError();
        return (-1);
    }
    
    

    pMapShelves = MapViewOfFile(hMapFileShelves, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if(pMapShelves == NULL){
        cerr << "MapViewOfFile error " << GetLastError();
        CloseHandle(hMapFileShelves);
        CloseHandle(hMapFileValability);
        CloseHandle(hMapFilePrices);
        
     }
    shelvesArray = static_cast<DWORD*>(pMapShelves);

    pMapValability = MapViewOfFile(hMapFileValability, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if(pMapValability == NULL){
        cerr << "MapViewOfFile error " << GetLastError();
        CloseHandle(hMapFileShelves);
        CloseHandle(hMapFileValability);
        CloseHandle(hMapFilePrices);
        UnmapViewOfFile(pMapShelves);
        
     }
    valabilityArray = static_cast<DWORD*>(pMapValability);

    pMapPrices = MapViewOfFile(hMapFilePrices, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if(pMapPrices == NULL){
        cerr << "MapViewOfFile error " << GetLastError();
        CloseHandle(hMapFileShelves);
        CloseHandle(hMapFileValability);
        CloseHandle(hMapFilePrices);
        UnmapViewOfFile(pMapShelves);
        UnmapViewOfFile(pMapValability);
     }
    pricesArray = static_cast<DWORD*>(pMapPrices);

    size_t arraySize = 10000;  //init the arrays with 0xFFFFFFFF because we used anonymous mapping and the default value is 0x00000000
    for (size_t i = 0; i < arraySize; i++){
        shelvesArray[i] = 0xFFFFFFFF;
        pricesArray[i] = 0xFFFFFFFF;
        valabilityArray[i] = 0xFFFFFFFF;
    }

    Sleep(100);

    ZeroMemory(si, sizeof(si));
    for (int i = 0; i < 3; i++) {
        si[i].cb = sizeof(STARTUPINFO);
    }

    
    for (int i = 0; i < 3; i++) {
        if (!CreateProcess(
                programs[i],   
                NULL,          
                NULL,          
                NULL,          
                FALSE,         
                0,             
                NULL,          
                NULL,           
                &si[i],        
                &pi[i]))       
        {
            cerr << "CreateProcess failed for " << programs[i] << " with error " << GetLastError() << endl;
        }
    }

    
    HANDLE hProcesses[3] = {pi[0].hProcess, pi[1].hProcess, pi[2].hProcess};
    SetEvent(hEventSoldDone);
    WaitForMultipleObjects(3, hProcesses, TRUE, INFINITE);

    
    for (int i = 0; i < 3; i++) {
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
    }

    CloseHandle(hMutex);
    CloseHandle(hEventDepoDone);
    CloseHandle(hEventSoldDone);
    CloseHandle(hEventDonationDone);
    CloseHandle(hMapFileShelves);
    CloseHandle(hMapFileValability);
    CloseHandle(hMapFilePrices);
    UnmapViewOfFile(pMapShelves);
    UnmapViewOfFile(pMapValability);
    UnmapViewOfFile(pMapPrices);

    return 0;


}