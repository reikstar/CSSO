#include "utils.h"
#include "tlhelp32.h"
#define fileSize (sizeof(DWORD) * 10000)


int main(){
    HANDLE hMapFileShelves, hMapFileValability, hMapFilePrices,hFile;

    createDirectoryRecursively("C:\\Facultate\\CSSO\\Week4\\Reports\\Summary");
    
    createFile("C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\logs.txt", FILE_APPEND_DATA);
    
    hMapFileShelves = createMapping("MarketShelves", PAGE_READWRITE, 0, fileSize);
    hMapFileValability = createMapping("MarketValability", PAGE_READWRITE, 0 , fileSize);
    hMapFilePrices = createMapping("ProductPrices", PAGE_READWRITE, 0 , fileSize);

    Sleep(100 * 1000);



    CloseHandle(hMapFileShelves);
    CloseHandle(hMapFileValability);
    CloseHandle(hMapFilePrices);



}