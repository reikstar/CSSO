#include <iostream>
#include <windows.h>
#include <sstream>
#include <string>
#include <vector>
#include <wininet.h>


using namespace std;


string HTTP_GET_Response_ToString (const string &url, HINTERNET session, size_t MAX_BYTES_TO_READ){

    DWORD error;
    vector <HINTERNET> HandleVector;
    char buffer[MAX_BYTES_TO_READ];

    HandleVector.push_back(session);

    HINTERNET hOpenUrl = InternetOpenUrl(session, 
                                         url.c_str(), NULL, 0, 
                                         INTERNET_FLAG_RELOAD, 0);

        if(hOpenUrl == NULL){
            error = GetLastError();
            cerr << "InternetOpenUrl error " << error;

            for(int i = HandleVector.size() - 1; i >= 0; i --){

                InternetCloseHandle(HandleVector[i]);
            }
            throw runtime_error("InternetOpenUrl error " + to_string(error));
        }

        HandleVector.push_back(hOpenUrl);

        memset(buffer,'\0', MAX_BYTES_TO_READ);

        DWORD BYTES_READ = 0;

        bool successfulRead = InternetReadFile(hOpenUrl, 
                                    buffer, 
                                    MAX_BYTES_TO_READ, 
                                    &BYTES_READ) == TRUE;

        if(!successfulRead){
            
            error = GetLastError();
                
            for(int i = HandleVector.size() - 1; i >= 0; i --){

                InternetCloseHandle(HandleVector[i]);

            }
            throw runtime_error("InternetReadFile error " + to_string(error));
        }

        if(BYTES_READ > 0){

            buffer[BYTES_READ] = '\0';   
        }

        string requestContent(buffer, BYTES_READ);

        InternetCloseHandle(hOpenUrl);

        return requestContent;
}

void createDirectoryRecursively(const string &directory){  
     const string separators("\\");
  
    DWORD fileAttributes = GetFileAttributesA(directory.c_str());
    if(fileAttributes == INVALID_FILE_ATTRIBUTES) // if the directory does not exist
    { 
        size_t backslashIndex = directory.find_last_of(separators);
        if(backslashIndex != string::npos){
            createDirectoryRecursively(directory.substr(0,backslashIndex));
      }   // string::npos is used to represent an invalid or nonexistent position or size within a string
        if(CreateDirectoryA(directory.c_str(), nullptr ) == false){
            throw runtime_error("Directory could not be created");
        }


    }
    else {
        bool isDirectory = (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        
        if(!isDirectory){
            throw runtime_error("Could not create directory because a file with the same name exists");

        }
    }
    
}

void appendToFile(const string &path, const string &buffer){

        HANDLE handle = CreateFileA(
            path.c_str(),
            FILE_APPEND_DATA,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        if(handle == INVALID_HANDLE_VALUE){
            cout << "Unable to open file " << path << " Error " << GetLastError() << endl;
            return;
        }

        DWORD dwBytesToWrite = (DWORD)buffer.size();
        DWORD dwBytesWritten = 0;

        bool successfulWrite = WriteFile(handle, buffer.c_str(), dwBytesToWrite, &dwBytesWritten, NULL) == 1;
        if(!successfulWrite){
            cout << "Error writing to the file" << " (" << GetLastError() << ")" << endl;
    
        }
        CloseHandle(handle);

}

void WriteToFile(const string &path, const string &buffer){

        HANDLE handle = CreateFileA(
            path.c_str(),
            GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        if(handle == INVALID_HANDLE_VALUE){
            cout << "Unable to open file " << path << " Error " << GetLastError() << endl;
            return;
        }

        DWORD dwBytesToWrite = (DWORD)buffer.size();
        DWORD dwBytesWritten = 0;

        bool successfulWrite = WriteFile(handle, buffer.c_str(), dwBytesToWrite, &dwBytesWritten, NULL) == 1;
        if(!successfulWrite){
            cout << "Error writing to the file" << " (" << GetLastError() << ")" << endl;
    
        }
        CloseHandle(handle);

}

void createFile(const string &path, DWORD DesiredAccess){
    HANDLE hFile = CreateFileA(
      path.c_str(),
      DesiredAccess,
      0,
      NULL,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      NULL
   );
   if(hFile == INVALID_HANDLE_VALUE ){
    cerr << "Error creating file: " << path << " (" << GetLastError() << ")" << endl;
   }

   CloseHandle(hFile);        
    
    }

HANDLE createMapping(const string &name, DWORD DesiredAccess, DWORD highOrderBytes, DWORD lowOrderBytes){
    HANDLE hFileMapping = CreateFileMapping(
                          INVALID_HANDLE_VALUE,
                          NULL,
                          DesiredAccess,
                          highOrderBytes,
                          lowOrderBytes,
                          name.c_str());
    if(hFileMapping == NULL){
       throw runtime_error("Could not create file mapping object. Error " + GetLastError());
        
    }

    return hFileMapping;
    
}

string fileToString(const string &path, int MAX_BYTES_TO_READ){
     HANDLE hFile;
     hFile = CreateFile(
                path.c_str(),
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL);

        if(hFile == INVALID_HANDLE_VALUE){
            throw runtime_error("Error opening file: " + path + " (" + to_string(GetLastError()) + ")\n"); 
       
        }

        char buffer[MAX_BYTES_TO_READ];
        memset(buffer, '\0', MAX_BYTES_TO_READ);
        
        DWORD bytesRead;
        bool successfulRead = ReadFile(hFile, buffer, MAX_BYTES_TO_READ-1, &bytesRead, NULL) != 0;

        if(!successfulRead){
            throw runtime_error("Error reading file: " + path + " (" + to_string(GetLastError()) + ")\n"); 
        }
    
        CloseHandle(hFile);
        if(bytesRead > 0 && bytesRead <= MAX_BYTES_TO_READ-1){
            buffer[bytesRead] = '\0';

        }
        else{
            if(bytesRead > MAX_BYTES_TO_READ-1){
                buffer[bytesRead] = '\0';
            }
            
        }

          string fileContent(buffer, bytesRead);
          
          return fileContent;
}