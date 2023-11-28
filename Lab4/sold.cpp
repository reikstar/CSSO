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

    cout << "sold started\n";
    HANDLE hMapShelves, hMapValability, hMapPrices, hFile, hMutex;
    LPVOID pMapShelves, pMapValability, pMapPrices;
    DWORD* shelvesArray;
    DWORD* valabilityArray;
    DWORD* pricesArray;
    WIN32_FIND_DATAA ffd;
    vector<string> filenames;
    vector<int> numbers;
    int id_produs, expires_in, shelve_id, product_price;

    HANDLE hEventThis = OpenEvent(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, "DonationDone");
    HANDLE hEventNext = OpenEvent(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, "SoldDone");

    if (!hEventThis || !hEventNext) {
        cerr << "Failed to open events from sold.exe. Error "  << GetLastError();
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


    hFile = FindFirstFile("C:\\Users\\Asihma\\CSSO\\Lab4\\sold\\*", &ffd);  //going through the files
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

    int old_value = 0;             
    int x = 2;                       
    for(auto & file : filenames){
       bool NoErrorForTheDay = TRUE;
       WaitForSingleObject(hEventThis, INFINITE);
       string path = "C:\\Users\\Asihma\\CSSO\\Lab4\\sold\\" + file;
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
            string number = line;
            numbers.push_back(stoi(number));
            
       }

       DWORD dwWaitResponse = WaitForSingleObject(hMutex, INFINITE);

       switch (dwWaitResponse){

            case WAIT_OBJECT_0:
                std::cout << "Sold got ownership of mutex at day " << x  << endl;
                for(int i = 0; i < numbers.size(); i ++){

                shelve_id = numbers[i];
            
                if(shelvesArray[shelve_id] == 0xFFFFFFFF){
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
                    CloseHandle(hMapShelves);
                    CloseHandle(hMapValability);
                    CloseHandle(hMapPrices);
                    UnmapViewOfFile(pMapShelves);
                    UnmapViewOfFile(pMapValability);
                    UnmapViewOfFile(pMapPrices);
                }
                CloseHandle(hFile);
                if(NoErrorForTheDay){
                    string error = "Errors from sold day " + to_string(x) + "\n";
                    appendToFile("C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\errors.txt", error.c_str());
                    NoErrorForTheDay = FALSE;
                }
                string error = "S-a incercat vanzarea produsului de pe raftul " + to_string(shelve_id) + " ce nu contine nimic.\n";
                 
                appendToFile("C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\errors.txt", error.c_str());
                }

                else{
                    if(valabilityArray[shelvesArray[shelve_id]] != 0){
                        id_produs = shelvesArray[shelve_id];
                        product_price = pricesArray[id_produs];
                        expires_in = valabilityArray[id_produs];

                        int new_value = old_value + product_price;

                        string logsLine = "S-a vandut produsul " + to_string(id_produs) + " de pe raftul "  
                            + to_string(shelve_id) + " cu " + to_string(expires_in) 
                            + " zile inainte de a fi donat cu pretul de " + to_string(product_price) + "\n";
                        appendToFile("C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\logs.txt", logsLine);
                        WriteToFile ("C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\sold.txt", to_string(new_value));

                        old_value = new_value;

                        valabilityArray[id_produs] = 0xFFFFFFFF;
                        pricesArray[id_produs] = 0xFFFFFFFF;
                        shelvesArray[shelve_id] = 0xFFFFFFFF;
                        }
                    }
            
                }
                 if(ReleaseMutex(hMutex)){
                    std::cout << "sold released mutex on day " << x << endl;
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

        cout << "day" << x << "done from sold\n";
        x++;
        numbers.clear();
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