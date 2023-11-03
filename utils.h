#include <iostream>
#include <windows.h>
#include <sstream>
#include <string>
#include <vector>

using namespace std;


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
            cout << "Unable to open file " << path << endl;
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