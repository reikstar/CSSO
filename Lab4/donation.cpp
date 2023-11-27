#include "utils.h"

int main(){

    cout << "donation started\n";
    HANDLE hMapShelves, hMapValability, hMapPrices, hFile, hMutex;
    LPVOID pMapShelves, pMapValability, pMapPrices;
    DWORD* shelvesArray;
    DWORD* valabilityArray;
    DWORD* pricesArray;


    HANDLE hEventThis = OpenEvent(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, "DepoDone");
    HANDLE hEventNext = OpenEvent(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, "DonationDone");

    if (!hEventThis || !hEventNext) {
        cerr << "Failed to open events in donation.exe. Error "  << GetLastError();
        return (-1);
    }

    hMutex = OpenMutex(
        SYNCHRONIZE,
        FALSE,
        "mutex");

    if(hMutex == NULL){
        cerr << "CreateMutex error " << GetLastError();
        return (-1);
    }
    // open all file mappings.
    hMapShelves = OpenFileMapping(
                    FILE_MAP_ALL_ACCESS,
                    FALSE,
                    "MarketShelves");
    if(hMapShelves == NULL){
        cerr << "Could not open file mapping object. Error " << GetLastError();
        return (-1);
    }

    hMapValability = OpenFileMapping(
                    FILE_MAP_ALL_ACCESS,
                    FALSE,
                    "MarketValability");
    if(hMapValability == NULL){
        cerr << "Could not open file mapping object. Error " << GetLastError();
        CloseHandle(hMapShelves);
        return (-1);
    }

    hMapPrices = OpenFileMapping(
                    FILE_MAP_ALL_ACCESS,
                    FALSE,
                    "ProductPrices");
    if(hMapPrices == NULL){
        cerr << "Could not open file mapping object. Error " << GetLastError();
        CloseHandle(hMapShelves);
        CloseHandle(hMapValability);
        return (-1);
    }  
    // opening mappings done. doing views for each one now
    pMapShelves = MapViewOfFile(hMapShelves, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if(pMapShelves == NULL){
        cerr << "MapViewOfFile error " << GetLastError();
        CloseHandle(hMapShelves);
        CloseHandle(hMapValability);
        CloseHandle(hMapPrices);
    }
    shelvesArray = static_cast<DWORD*>(pMapShelves);

    pMapValability = MapViewOfFile(hMapValability, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if(pMapValability == NULL){
        cerr << "MapViewOfFile error " << GetLastError();
        CloseHandle(hMapShelves);
        CloseHandle(hMapValability);
        CloseHandle(hMapPrices);
        UnmapViewOfFile(pMapShelves);
    }
    valabilityArray = static_cast<DWORD*>(pMapValability);

    pMapPrices = MapViewOfFile(hMapPrices, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if(pMapPrices == NULL){
        cerr << "MapViewOfFile error " << GetLastError();
        CloseHandle(hMapShelves);
        CloseHandle(hMapValability);
        CloseHandle(hMapPrices);
        UnmapViewOfFile(pMapShelves);
        UnmapViewOfFile(pMapValability);
    }
    pricesArray = static_cast<DWORD*>(pMapPrices);

    int old_value = 0;
    for (int i = 1; i <= 29; i++){  //since we have 29 days, we are going to loop 29 times for each day.
        WaitForSingleObject(hEventThis, INFINITE);
        
        DWORD dwWaitResponse = WaitForSingleObject(hMutex, INFINITE);

        switch (dwWaitResponse){
            case WAIT_OBJECT_0:
                std::cout << "Donation got ownership of mutex at day " << i << endl;
                for (size_t id_produs = 0; id_produs < 10000; id_produs ++){
                if(valabilityArray[id_produs] == 0){
                    int new_value = old_value + pricesArray[id_produs];
                    old_value = new_value;
                    string logsLine = "Produsul " + to_string(id_produs) + " a fost donat.\n";

                    WriteToFile("C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\donations.txt", to_string(new_value));  
                    appendToFile("C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\logs.txt", logsLine.c_str());

                    valabilityArray[id_produs] = 0xFFFFFFFF;
                    pricesArray[id_produs] = 0xFFFFFFFF;

                    for(size_t shelve_id = 0; shelve_id < 10000; shelve_id ++){
                        if(shelvesArray[shelve_id] == id_produs){
                            shelvesArray[shelve_id] = 0xFFFFFFFF;
                        }
                    }

                }

                if(valabilityArray[id_produs] > 0 && valabilityArray[id_produs] < 0xFFFFFFFF){
                    valabilityArray[id_produs] --;
                }
            }
            
            
            if(ReleaseMutex(hMutex)){

                std::cout << "deposit released mutex on day " << i << endl;
            }

            else{
                cerr << "Release mutex error " << GetLastError();
                return (-1);
            }
            
            break;

        case WAIT_ABANDONED:
            cerr << "mutex abandoned";
            return (-1); 
        
        
        
        }
        cout << "day" << i << "done from donation\n";
        Sleep(100);
        SetEvent(hEventNext);

    }

        

    CloseHandle(hMapShelves);
    CloseHandle(hMapValability);
    CloseHandle(hMapPrices);
    UnmapViewOfFile(pMapShelves);
    UnmapViewOfFile(pMapValability);
    UnmapViewOfFile(pMapPrices);

    return 0;
}
