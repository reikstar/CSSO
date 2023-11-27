#include "utils.h"
#include "algorithm"
#include <sstream>


const int MAX_BYTES_TO_READ = 6144; // A file has maximum 4kb, so 6144 bytes would be enough


bool dayCompare(const string &file1, const string &file2){
    int day1, day2;
    sscanf(file1.c_str(),"%*d.%*d.%d.txt", &day1);
    sscanf(file2.c_str(),"%*d.%*d.%d.txt", &day2);
    return day1 < day2;
}



int main(){

    cout << "deposit started\n";
    HANDLE hMapShelves, hMapValability, hMapPrices, hFile, hMutex, hEventThis, hEventNext;
    LPVOID pMapShelves, pMapValability, pMapPrices;
    DWORD* shelvesArray;
    DWORD* valabilityArray;
    DWORD* pricesArray;
    WIN32_FIND_DATAA ffd;
    vector<string> filenames;
    vector<int> numbers;
    int id_produs, expires_in, shelve_id, product_price;

    hEventThis = OpenEvent(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, "SoldDone");
    hEventNext = OpenEvent(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, "DepoDone");

    if (!hEventThis || !hEventNext) {
        cerr << "Failed to open events from deposit.exe. Error  "  << GetLastError();
        return (-1);
    }

    hMutex = CreateMutex(
        NULL,
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

    


    hFile = FindFirstFile("C:\\Users\\Asihma\\CSSO\\Lab4\\deposit\\*", &ffd);  //going through the files
    if(hFile == INVALID_HANDLE_VALUE){
        cerr << "Could not find first file. Error " << GetLastError();
        CloseHandle(hMapShelves);
        CloseHandle(hMapValability);
        CloseHandle(hMapPrices);
        UnmapViewOfFile(pMapShelves);
        UnmapViewOfFile(pMapValability);
        UnmapViewOfFile(pMapPrices);
        return (-1);
    }
    do{

        if(strcmp(ffd.cFileName, ".") != 0 && strcmp(ffd.cFileName, "..") != 0){
            filenames.push_back(ffd.cFileName);
        }
        

    }while(FindNextFile(hFile, &ffd) != 0);
    FindClose(hFile);

    sort(filenames.begin(), filenames.end(), dayCompare);  //FindNextFile would not give them in a sorted order, so we have to do it manually.
                                                           //Custom comparator to order by the day of the month.
                                        
    for(auto & file : filenames){

      
      WaitForSingleObject(hEventThis, INFINITE); // The order of them in a ring would be deposit-sold-donation. 
                                                  // First iteration would be only deposit-donation since sold has -1 day
       cout << "day done from deposit\n";                                 
       string path = "C:\\Users\\Asihma\\CSSO\\Lab4\\deposit\\" + file;
       string fileContent;
       try
       {
        fileContent = fileToString(path, MAX_BYTES_TO_READ);
       }
       catch(exception& e)
       {
        cerr << e.what() << '\n';
        CloseHandle(hMapShelves);
        CloseHandle(hMapValability);
        CloseHandle(hMapPrices);
        UnmapViewOfFile(pMapShelves);
        UnmapViewOfFile(pMapValability);
        UnmapViewOfFile(pMapPrices);
        return (-1);
       }
       istringstream iss(fileContent);
       string line;

       while(getline(iss, line)){
            istringstream lineStream(line);
            string number;

            while(getline(lineStream, number, ',')){
                numbers.push_back(stoi(number));
            }
       }
       if(WaitForSingleObject(hMutex, INFINITE) == WAIT_ABANDONED){
         cout << "Abandoned mutex" << endl;
         continue;
       }
       for(int i = 0; i < numbers.size(); i += 4){
            
            id_produs = numbers[i];         
            expires_in = numbers[i + 1];
            shelve_id = numbers[i + 2];
            product_price = numbers[i + 3];
            

            if(shelvesArray[shelve_id] != 0xFFFFFFFF){
                hFile = CreateFile(
                        "C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\errors.txt",
                        GENERIC_READ | GENERIC_WRITE,
                        0,
                        NULL,
                        OPEN_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);
                if(hFile == INVALID_HANDLE_VALUE){
                    cerr << "Could not create/open file. Error " << GetLastError();
                    return (-1);
                    CloseHandle(hMutex);
                    CloseHandle(hMapShelves);
                    CloseHandle(hMapValability);
                    CloseHandle(hMapPrices);
                    UnmapViewOfFile(pMapShelves);
                    UnmapViewOfFile(pMapValability);
                    UnmapViewOfFile(pMapPrices);
                }
                CloseHandle(hFile);
                
                string error = "S-a incercat adaugarea produsului " + to_string(id_produs) + " pe raftul "
                + to_string(shelve_id) + " care este deja ocupat de " 
                + to_string(shelvesArray[shelve_id]) + "\n"; 

                appendToFile("C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\errors.txt", error.c_str());


            }
            else{
                
                shelvesArray[shelve_id] = id_produs;
                valabilityArray[id_produs] = expires_in;        //writing in memory map
                pricesArray[id_produs] = product_price;
                string logsLine = "Am adaugat pe raftul " + to_string(shelve_id) + " produsul "  
                + to_string(id_produs) + " ce are o valabilitate de " + to_string(expires_in) 
                + " zile si un pret de " + to_string(product_price) + "\n";
                appendToFile("C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\logs.txt", logsLine.c_str());
            }

       }

       ReleaseMutex(hMutex);
       SetEvent(hEventNext);       
}
    
    
    
    CloseHandle(hEventNext);
    CloseHandle(hMutex);
    CloseHandle(hMutex);
    CloseHandle(hMapShelves);
    CloseHandle(hMapValability);
    CloseHandle(hMapPrices);
    UnmapViewOfFile(pMapShelves);
    UnmapViewOfFile(pMapValability);
    UnmapViewOfFile(pMapPrices);

    return 0;

}