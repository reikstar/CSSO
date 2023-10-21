#include <iostream>
#include <windows.h>
#include <sstream>
#include <string>
using namespace std;
const int MAX_BYTES_TO_READ = 0x4000;
LPCSTR sumar = "C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate\\sumar.txt";

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

void wordSearch (const string &word, const string &filePath){
    HANDLE hFile = CreateFileA(filePath.c_str(), 
    GENERIC_READ,
    FILE_SHARE_READ,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL
    );
    
    if(hFile == INVALID_HANDLE_VALUE){
        cerr << "Error opening file: " << filePath << " (" << GetLastError() << ")" << endl;
        return;
    }
    
    char buffer[MAX_BYTES_TO_READ];
    memset(buffer, '\0', MAX_BYTES_TO_READ);
    
    DWORD bytesRead;
    bool successfulRead = ReadFile(hFile, buffer, MAX_BYTES_TO_READ-1, &bytesRead, NULL) != 0;

    if(!successfulRead){
        cout << "Error reading file: " << filePath << " (" << GetLastError() << ")" << endl;
        return;
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

    size_t foundIndex = fileContent.find(word);
    if(foundIndex != string::npos){
        HANDLE hSumar = CreateFileA(
            sumar,
            FILE_APPEND_DATA,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        if(hSumar == INVALID_HANDLE_VALUE){
            cout << "Unable to open file " << sumar << endl;
            return;
        }

        string strIndex = to_string(foundIndex);
    
        string writeContent = "Found '" + word + "' in " + filePath + " at index " + strIndex + '\n';
        cout << writeContent;

        DWORD dwBytesToWrite = (DWORD)writeContent.size();
        DWORD dwBytesWritten = 0;

        bool successfulWrite = WriteFile(hSumar, writeContent.c_str(), dwBytesToWrite, &dwBytesWritten, NULL) == 1;
        if(!successfulWrite){
            cout << "Error writing to the file" << " (" << GetLastError() << ")" << endl;
    
        }
        CloseHandle(hSumar);

    }

    

    
}

void recursiveSearch(const string &word, const string &directory){

    WIN32_FIND_DATAA ffd;
    HANDLE hFIND = FindFirstFileA((directory + "\\*").c_str(), &ffd);
    if(hFIND == INVALID_HANDLE_VALUE){
        cout << GetLastError();
        return;
    }
    do {
        if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            if (strcmp(ffd.cFileName, ".") != 0 && strcmp(ffd.cFileName, "..") != 0){      //exclude current directory and parent directory
                recursiveSearch(word, directory + "\\" + ffd.cFileName);
            } 
        }

        else{ 
            if (strcmp(ffd.cFileName, "sumar.txt") != 0){
                wordSearch(word ,directory + "\\" + ffd.cFileName);
        }
            
        }
    } while(FindNextFileA(hFIND, &ffd) != 0);
    FindClose(hFIND);



}

int main(){
   createDirectoryRecursively("C:\\Facultate\\CSSO\\Laboratoare\\Week1");
   createDirectoryRecursively("C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate");
   HANDLE hFile = CreateFileA(
      sumar,
      FILE_APPEND_DATA,
      0,
      NULL,
      OPEN_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      NULL
   );
   if(hFile == INVALID_HANDLE_VALUE ){
    cerr << "Error creating file: " << sumar << " (" << GetLastError() << ")" << endl;
    return GetLastError();
   }

   CloseHandle(hFile);

  string word,directory;

   cout <<"Word to search for:";
   cin >> word;
   
   cout <<"Directory to search for:";
   cin >> directory;
   
   //Replacing single \ with double in directory string
   size_t pos = 0;
    while ((pos = directory.find("\\", pos)) != string::npos) {
        directory.replace(pos, 1, "\\\\");
        pos += 2; 
    }
 
   DWORD fileAttributes = GetFileAttributesA(directory.c_str());
   
   if(fileAttributes == INVALID_FILE_ATTRIBUTES){
        throw runtime_error(directory + " directory does not exist.");
   }

   recursiveSearch(word,directory);

    

   return 0;
}