#include "utils.h"
#include "tlhelp32.h"
#define fileSize (sizeof(DWORD) * 10000)


int main(){
    HANDLE hMapFileShelves, hMapFileValability, hMapFilePrices,hFile;
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

    Sleep(100 * 1000);



    CloseHandle(hMapFileShelves);
    CloseHandle(hMapFileValability);
    CloseHandle(hMapFilePrices);
    UnmapViewOfFile(pMapShelves);
    UnmapViewOfFile(pMapValability);
    UnmapViewOfFile(pMapPrices);




}