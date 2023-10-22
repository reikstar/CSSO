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

void writeToFile(const string &path, const string &buffer){

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

void createFile(const string &path){
    HANDLE hFile = CreateFileA(
      path.c_str(),
      GENERIC_WRITE,
      0,
      NULL,
      OPEN_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      NULL
   );
   if(hFile == INVALID_HANDLE_VALUE ){
    cerr << "Error creating file: " << path << " (" << GetLastError() << ")" << endl;
   }

   CloseHandle(hFile);        
    
    }
void createRegistryKey(HKEY hRootKey, const string &keyPath){
    HKEY hKey;
    LONG result = RegCreateKeyExA(hRootKey, keyPath.c_str(), 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);

    if(result != ERROR_SUCCESS){
        cerr << "Error creating key: " << keyPath << "(Error code: " << result << ")" << endl;
        RegCloseKey(hKey);
    }

    else{
        RegCloseKey(hKey);
    }
}






int main(){

    vector<HKEY> keyList = {HKEY_LOCAL_MACHINE, HKEY_CURRENT_CONFIG, HKEY_CURRENT_USER};
    DWORD subKeysCount, maxSubKeyLen, filesCreatedCount;
    FILETIME lastWriteTime;
    SYSTEMTIME systemTime;
    LONG result, value;
    vector<string> hiveFiles = {"HKLM.txt", "HKCC.txt", "HKCU.txt"};
    LARGE_INTEGER filesize;
    string writeContent;
    HKEY hkcuSoftware;
    HKEY hKey;
    createDirectoryRecursively("C:\\Facultate\\CSSO\\Laboratoare\\Week2\\InstalledSoftware");
    createDirectoryRecursively("C:\\Facultate\\CSSO\\Laboratoare\\Week2\\Rezultate");
    
    for(int i = 0; i < hiveFiles.size(); i++){  // files coresponding to hives creation
        string file = "C:\\Facultate\\CSSO\\Laboratoare\\Week2\\Rezultate\\" + hiveFiles[i];

        createFile(file);
    }

    for (int i = 0; i < keyList.size(); i++){
        result = RegQueryInfoKeyA(keyList[i], NULL, NULL, NULL, &subKeysCount, &maxSubKeyLen, NULL, NULL, NULL, NULL, NULL, &lastWriteTime);
    
        if (result == ERROR_SUCCESS) {
            FileTimeToSystemTime(&lastWriteTime, &systemTime);
            string timeData;
            timeData = 
                to_string(systemTime.wYear) + "/"
                + to_string(systemTime.wMonth) + "/"
                + to_string(systemTime.wDay) + " "
                + to_string(systemTime.wHour) + ":"
                + to_string(systemTime.wMinute) + ":"
                + to_string(systemTime.wSecond);
            string writeContent = "lpcSubKeys:" + to_string(subKeysCount) 
                                                + "\nlpcMaxSubKeyLen:" + to_string(maxSubKeyLen)
                                                + "\nlpftLastWriteTime:" + timeData;                //writing data to each fille 

            if (keyList[i] == HKEY_LOCAL_MACHINE){
                writeToFile("C:\\Facultate\\CSSO\\Laboratoare\\Week2\\Rezultate\\HKLM.txt", writeContent);
            }
            if (keyList[i] == HKEY_CURRENT_CONFIG){
                writeToFile("C:\\Facultate\\CSSO\\Laboratoare\\Week2\\Rezultate\\HKCC.txt", writeContent);
            }
            if (keyList[i] == HKEY_CURRENT_USER){
                writeToFile("C:\\Facultate\\CSSO\\Laboratoare\\Week2\\Rezultate\\HKCU.txt", writeContent);
            }
        }

        else{
            cerr << "Error " << " (" << GetLastError() << ")" << endl;
        }

    }
    LPCSTR sumar = "C:\\Facultate\\CSSO\\Laboratoare\\Week2\\Rezultate\\sumar.txt";
    HANDLE hFile = CreateFileA(
      sumar,
      GENERIC_WRITE,
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

   WIN32_FIND_DATAA ffd;
   
   HANDLE hFind = FindFirstFileA(LPCSTR("C:\\Facultate\\CSSO\\Laboratoare\\Week2\\Rezultate\\*"), &ffd);
   if(hFind == INVALID_HANDLE_VALUE){
        cout << GetLastError();
        return 0;
    }


   do{

        if (strcmp("sumar.txt", ffd.cFileName) == 0 || strcmp(".", ffd.cFileName) == 0 || strcmp("..", ffd.cFileName) == 0 ){
            continue;
        }
        filesize.LowPart = ffd.nFileSizeLow;
        filesize.HighPart = ffd.nFileSizeHigh;

         writeContent = writeContent + string(ffd.cFileName) + " " + to_string(filesize.QuadPart) + " bytes\n";
       

   }while(FindNextFileA(hFind, &ffd) != 0);
    writeToFile("C:\\Facultate\\CSSO\\Laboratoare\\Week2\\Rezultate\\sumar.txt", writeContent);
    FindClose(hFind);

     value = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE", 0, KEY_READ, &hkcuSoftware);
    
    if (value == ERROR_SUCCESS) {
        DWORD subKeyIndex = 0;
        CHAR subKeyName[MAX_PATH];
        DWORD subKeyNameSize = MAX_PATH;
        
        while (true){
            result = RegEnumKeyExA(hkcuSoftware, subKeyIndex, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL);

            if(result == ERROR_SUCCESS){
            
                writeContent ="C:\\Facultate\\CSSO\\Laboratoare\\Week2\\InstalledSoftware\\";
                writeContent.append(subKeyName);
                createFile(writeContent);
                filesCreatedCount++;
                subKeyIndex++;
                subKeyNameSize = MAX_PATH;
            }
            else if (result == ERROR_NO_MORE_ITEMS){
                break;
            }
            else{
                cerr << "Error enumerating subkeys: " << result << endl;
                break;
            }


        }

       RegCloseKey(hkcuSoftware);
    }
    
    else{
        RegCloseKey(hkcuSoftware);
        cerr << "Error opening key HKCU\\SOFTWARE " << result << endl;
    }

    createRegistryKey(HKEY_CURRENT_USER, "SOFTWARE\\CSSO");
    createRegistryKey(HKEY_CURRENT_USER, "SOFTWARE\\CSSO\\Week2");

    value = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\CSSO\\Week2", 0, KEY_SET_VALUE, &hKey);

    if(value == ERROR_SUCCESS){

        writeContent = "C:\\Facultate\\CSSO\\Laboratoare\\Week2\\InstalledSoftware";
        value = RegSetValueExA(hKey, "StringValue", 0, REG_SZ, (const BYTE*)writeContent.c_str(), writeContent.size() + 1);

        if(value != ERROR_SUCCESS){
           cerr << "Error setting REG_SZ value: StringValue (Error code: " << value << ")";
        }

        value = RegSetValueExA(hKey, "DwordValue", 0, REG_DWORD, (const BYTE*)&filesCreatedCount, sizeof(DWORD));

        if(value != ERROR_SUCCESS){
            cerr << "Error setting REG_DWORD value: DwordValue (Error code: " << value << ")";
        }

    }

    RegCloseKey(hKey);



    return 0;
}
