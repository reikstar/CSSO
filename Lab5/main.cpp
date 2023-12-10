
#include "utils.h"
#include <wininet.h>
#include <sstream>
#define MAX_BYTES_TO_READ 0x4000

int main(){
    WIN32_FIND_DATAA ffd;
    HINTERNET hInternetOpen, hOpenUrl, hRequest, hConnect;
    HANDLE hFile;
    vector <HINTERNET> HandleVector;
    PCTSTR acceptTypes[] = {"text/*", NULL};
    char buffer[MAX_BYTES_TO_READ];
    DWORD BYTES_READ = 0, error;
    int GET_COUNT = 0, POST_COUNT = 0, EX2_REQUESTS_COUNT = 0;
    ULONGLONG totalFilesSize = 0;

    string path_url = fileToString("C:\\Users\\Asihma\\CSSO\\Lab5\\url.txt", 1024);
    string NR_MATRICOL = fileToString("C:\\Users\\Asihma\\CSSO\\Lab5\\Nr_matricol.txt", 1024); 
    string serverUrl = path_url + NR_MATRICOL, method, lastGETResponse;

    hInternetOpen = InternetOpen(NR_MATRICOL.c_str(),
                                INTERNET_OPEN_TYPE_DIRECT,
                                NULL, NULL, 0);
    if (hInternetOpen == NULL){

        cerr << "InternetOpen error " << GetLastError();
        
        return (-1);
    }

    HandleVector.push_back(hInternetOpen);

    hOpenUrl = InternetOpenUrl(hInternetOpen, 
                               serverUrl.c_str(), NULL, 0, 
                               INTERNET_FLAG_RELOAD, 0);

    if(hOpenUrl == NULL){

        cerr << "InternetOpenUrl error " << GetLastError();

        for(int i = HandleVector.size() - 1; i >= 0; i --){

            InternetCloseHandle(HandleVector[i]);

        }

        return (-1);
    }


    memset(buffer, '\0', MAX_BYTES_TO_READ);

    bool successfulRead = InternetReadFile(hOpenUrl, 
                                           buffer, 
                                           MAX_BYTES_TO_READ, 
                                           &BYTES_READ) == TRUE;

    if(!successfulRead){
        cerr << "InternetReadFile error " << GetLastError();
        
        for(int i = HandleVector.size() - 1; i >= 0; i --){

            InternetCloseHandle(HandleVector[i]);

        }
        return (-1);
    }

    if(BYTES_READ > 0 && BYTES_READ <= MAX_BYTES_TO_READ-1){
            buffer[BYTES_READ] = '\0';

        }
        else{
            if(BYTES_READ > MAX_BYTES_TO_READ-1){
                buffer[BYTES_READ] = '\0';
            }
            
        }

        InternetCloseHandle(hOpenUrl);
        HandleVector.pop_back();

    string requestContent(buffer, BYTES_READ);

    GET_COUNT ++;

    createDirectoryRecursively("C:\\Facultate\\CSSO\\Week5");
    createDirectoryRecursively("C:\\Facultate\\CSSO\\Week5\\Downloads");

    createFile("C:\\Facultate\\CSSO\\Week5\\myconfig.txt", GENERIC_WRITE | GENERIC_READ);

    WriteToFile("C:\\Facultate\\CSSO\\Week5\\myconfig.txt", requestContent.c_str()); 

    //Getting data from NR_MATRICOL url done and transfered in myconfig.txt

    istringstream stream (requestContent);
    string line;

    while(getline(stream, line)){
        int pos = line.find(':');

        method = line.substr(0, pos);
        serverUrl = line.substr(pos + 1, string::npos);
        pos = line.find_last_of('/');
        
        string value = line.substr(pos + 1);
        

        if(serverUrl.find("dohomework_additional") != string::npos){
            
            string getResponse = HTTP_GET_Response_ToString(serverUrl.c_str(), hInternetOpen, MAX_BYTES_TO_READ);

            GET_COUNT ++;

            hFile = CreateFile(("C:\\Facultate\\CSSO\\Week5\\Downloads\\" + value + "_aditional.txt").c_str(),
                                   FILE_APPEND_DATA,
                                   0, 
                                   NULL,
                                   CREATE_NEW,
                                   FILE_ATTRIBUTE_NORMAL,
                                   NULL);
            CloseHandle(hFile);
            
            appendToFile(("C:\\Facultate\\CSSO\\Week5\\Downloads\\" + value + "_aditional.txt").c_str(), (getResponse + "\n").c_str());
            
            while (getResponse != "0"){
                
                getResponse = HTTP_GET_Response_ToString(("http://cssohw.herokuapp.com/dohomework_additional/" + getResponse).c_str(), hInternetOpen, MAX_BYTES_TO_READ);

                appendToFile(("C:\\Facultate\\CSSO\\Week5\\Downloads\\" + value + "_aditional.txt").c_str(), (getResponse + "\n").c_str());

                GET_COUNT ++;

            }
        }

        else{
            if(method == "GET"){ 
                
                hOpenUrl = InternetOpenUrl(hInternetOpen, 
                                           serverUrl.c_str(), NULL, 0, 
                                           INTERNET_FLAG_RELOAD, 0);

                if(hOpenUrl == NULL){
                    DWORD error = GetLastError();
                    cerr << "InternetOpenUrl error " << error;

                    for(int i = HandleVector.size() - 1; i >= 0; i --){

                        InternetCloseHandle(HandleVector[i]);
                    }
                    return (-1);
                }

                HandleVector.push_back(hOpenUrl);

                memset(buffer,'\0', MAX_BYTES_TO_READ);

                BYTES_READ = 0;

                bool successfulRead = InternetReadFile(hOpenUrl, 
                                           buffer, 
                                           MAX_BYTES_TO_READ, 
                                           &BYTES_READ) == TRUE;

                if(!successfulRead){
                    cerr << "InternetReadFile error " << GetLastError();
                    
                    for(int i = HandleVector.size() - 1; i >= 0; i --){

                        InternetCloseHandle(HandleVector[i]);

                    }
                    return (-1);
                }

                if(BYTES_READ > 0){

                    buffer[BYTES_READ] = '\0';   
                }

                string requestContent(buffer, BYTES_READ);
                lastGETResponse = requestContent;

                hFile = CreateFile(("C:\\Facultate\\CSSO\\Week5\\Downloads\\" + value).c_str(),
                                   FILE_APPEND_DATA,
                                   0, 
                                   NULL,
                                   CREATE_NEW,
                                   FILE_ATTRIBUTE_NORMAL,
                                   NULL);
                CloseHandle(hFile);
                
                appendToFile(("C:\\Facultate\\CSSO\\Week5\\Downloads\\" + value).c_str(), (requestContent + "\n").c_str());
            
                InternetCloseHandle(hOpenUrl);
                HandleVector.pop_back();

                GET_COUNT ++;
                EX2_REQUESTS_COUNT ++;
            }

            if(method == "POST"){
                
                string pathToPost = serverUrl.substr(serverUrl.find('/'));
                serverUrl = "cssohw.herokuapp.com";
                 
                hConnect = InternetConnect(hInternetOpen,
                                          serverUrl.c_str(),
                                          INTERNET_DEFAULT_HTTP_PORT,
                                          NULL, NULL,
                                          INTERNET_SERVICE_HTTP, 0, 0);
                
                if(hConnect == NULL){

                    error = GetLastError();

                    cerr << "InternetConnect error " << error;

                    InternetCloseHandle(hConnect);
                    return (-1);
                }

                HandleVector.push_back(hConnect);
                
                hRequest = HttpOpenRequest(hConnect,
                                           "POST",
                                           pathToPost.c_str(), NULL, NULL, NULL, 0, 0);
                
                if(hRequest == NULL){

                    error = GetLastError();

                    cerr << "HttpOpenRequest on POST error  " << error;

                    for (auto handle : HandleVector){
                        InternetCloseHandle(handle);
                    }
                    return (-1);
                        
                }

                string header = "Content-Type: application/x-www-form-urlencoded\r\n";
                string content = "id= " + string(NR_MATRICOL) + "&value=" + lastGETResponse;
                

                if( !HttpSendRequest(hRequest, header.c_str(), header.size(), (LPVOID)content.c_str(),content.size()) ){
                    
                    error = GetLastError();
                    cerr << "HttpSendRequest error " << error; 

                    for(auto handle : HandleVector){

                        InternetCloseHandle(handle);

                    }
                    return (-1);
                }

                POST_COUNT ++;
                EX2_REQUESTS_COUNT ++;

            }


        }
        
        
    }

    HANDLE hFIND = FindFirstFile("C:\\Facultate\\CSSO\\Week5\\Downloads\\*", &ffd);
    if(hFIND == INVALID_HANDLE_VALUE){

        error = GetLastError();

        cerr << "FindFirstFile error " << error;

        for(auto handle : HandleVector){

            InternetCloseHandle(handle);

        }
    
    }

    do{
        if(!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){

            ULONGLONG fileSize = (static_cast<ULONGLONG>(ffd.nFileSizeHigh) << 32) | ffd.nFileSizeLow;
            totalFilesSize += fileSize;
        }
    } while(FindNextFile(hFIND, &ffd) != 0);

    totalFilesSize = totalFilesSize / 1024;

    cout << totalFilesSize <<"KB" << endl;

    serverUrl = "cssohw.herokuapp.com";
                 
    hConnect = InternetConnect(hInternetOpen,
                                serverUrl.c_str(),
                                INTERNET_DEFAULT_HTTP_PORT,
                                NULL, NULL,
                                INTERNET_SERVICE_HTTP, 0, 0);
    
    if(hConnect == NULL){

        error = GetLastError();

        cerr << "InternetConnect error " << error;

        InternetCloseHandle(hConnect);
        return (-1);
    }

    HandleVector.push_back(hConnect);
    
    hRequest = HttpOpenRequest(hConnect,
                                "POST",
                                "/endhomework", NULL, NULL, NULL, 0, 0);
    
    
    
    if(hRequest == NULL){

        error = GetLastError();

        cerr << "HttpOpenRequest on POST2 error  " << error;

        for (auto handle : HandleVector){
            InternetCloseHandle(handle);
        }
        return (-1);
            
    }


    string header = "Content-Type: application/x-www-form-urlencoded\r\n";

    string content = "id=" + string(NR_MATRICOL) + "&total=" + 
                     to_string(EX2_REQUESTS_COUNT) + "&get=" + to_string(GET_COUNT) +
                     "&post=" + to_string(POST_COUNT + 1) + "&size=" + to_string(totalFilesSize);

    cout << content << endl;

    if( !HttpSendRequest(hRequest, header.c_str(), header.size(), (LPVOID)content.c_str(),content.size()) ){
                    
        error = GetLastError();
        cerr << "HttpSendRequest error " << error; 

        for(auto handle : HandleVector){

            InternetCloseHandle(handle);

        }
        return (-1);
    }

    for(auto handle : HandleVector){

        InternetCloseHandle(handle);

    }


    return 0;


    
}