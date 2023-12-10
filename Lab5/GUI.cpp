
#include <windows.h>
#include <windowsx.h>
#include <string>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>
#include "utils.h"



bool isProcessRunning = false;
// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("API");

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND h1Button, h2Button, h3Button, h4Button;
HWND myButton, editBox, outputEdit;

int WINAPI WinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ LPSTR     lpCmdLine,
   _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            MB_OK);
        return 1;
    }

    // Store instance handle in our global variable
    hInst = hInstance;

    // The parameters to CreateWindowEx explained:
    // WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 800, 600: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            MB_OK);
        return 1;
    }

    h1Button = CreateWindow(
        _T("BUTTON"),
        _T("URL"),
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 10, 100, 30,
        hWnd,
        (HMENU)1,
        hInstance,
        NULL);

    h2Button = CreateWindow(
        _T("BUTTON"),
        _T("Nr_Matricol"),
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 50, 100, 30,
        hWnd,
        (HMENU)2,
        hInstance,
        NULL);

    h3Button = CreateWindow(
        _T("BUTTON"),
        _T("Management"),
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 90, 100, 30,
        hWnd,
        (HMENU)3,
        hInstance,
        NULL);

    // h4Button = CreateWindow(
    //     _T("BUTTON"),
    //     _T("Output"),
    //     WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
    //     10, 130, 100, 30,
    //     hWnd,
    //     (HMENU)4,
    //     hInstance,
    //     NULL);

    // outputEdit = CreateWindow(
    // _T("EDIT"),
    // _T(""),
    // WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
    // 10, 300, 500, 200,
    // hWnd, NULL, NULL, NULL);  
       
    editBox = CreateWindow(
        _T("EDIT"),
        _T(""),
        WS_BORDER | WS_CHILD | WS_VISIBLE,
        10, 200, 500, 100,
        hWnd, NULL, NULL, NULL
    );

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("API");

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        RECT rect;
        GetClientRect(hWnd, &rect);
        rect.top = 0;
        rect.bottom /= 4;
        DrawText(hdc, greeting, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hWnd, &ps);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1:
            {
                char value[256];
                Edit_GetText(editBox, value, 256);
                if (strlen(value) == 0)
                {
                    MessageBox(hWnd, "No data was provided", "ERROR", MB_ICONERROR);
                }
                else
                {
                    int result = MessageBox(hWnd, value, "Are you sure you want to provide this URL?", MB_YESNO |
                        MB_TOPMOST | MB_ICONWARNING);
                    switch (result)
                    {
                    case IDYES:
                        {   string buffer = value;
                            MessageBox(hWnd, "URL provided", "YES", MB_ICONINFORMATION);
                            createFile("C:\\Users\\Asihma\\CSSO\\Lab5\\url.txt", GENERIC_ALL);
                            WriteToFile("C:\\Users\\Asihma\\CSSO\\Lab5\\url.txt", buffer.c_str());
                            
                        }
                        break;
                    case IDNO:
                        MessageBox(hWnd, "You pressed NO", "NO", MB_ICONINFORMATION);
                        break;
                    }
                }
            }
            break;

        case 2:
            {
                char value[256];
                Edit_GetText(editBox, value, 256);
                if (strlen(value) == 0)
                {
                    MessageBox(hWnd, "No data was provided", "ERROR", MB_ICONERROR);
                }
                else
                {
                    int result = MessageBox(hWnd, value, "Are you sure you want to provide this Number?", MB_YESNO |
                        MB_TOPMOST | MB_ICONWARNING);
                    switch (result)
                    {
                    case IDYES:
                        {
                            MessageBox(hWnd, "Number provided ", "YES", MB_ICONINFORMATION);
                            string buffer = value;
                
                            createFile("C:\\Users\\Asihma\\CSSO\\Lab5\\Nr_matricol.txt", GENERIC_ALL);
                            WriteToFile("C:\\Users\\Asihma\\CSSO\\Lab5\\Nr_matricol.txt", buffer.c_str());
                        }
                        break;
                    case IDNO:
                        MessageBox(hWnd, "You pressed NO", "NO", MB_ICONINFORMATION);
                        break;
                    }
                }
            }
            break;

        case 3:
            {
                if (!isProcessRunning)
                {
                    char testare[] = "C:\\Users\\Asihma\\CSSO\\Lab5\\main.exe";
                    STARTUPINFO si;
                    PROCESS_INFORMATION pi;
                    ZeroMemory(&si, sizeof(si));
                    si.cb = sizeof(si);
                    ZeroMemory(&pi, sizeof(pi));
                    if (!CreateProcess(
                            NULL,
                            testare,
                            NULL,
                            NULL,
                            FALSE,
                            0,
                            NULL,
                            NULL,
                            &si,
                            &pi))
                    {
                        isProcessRunning = true;
                        // Close handles to the child process and its primary thread
                        CloseHandle(pi.hProcess);
                        CloseHandle(pi.hThread);
                    }
                    else
                    {
                        std::cerr << "CreateProcess of Collect.exe error " << GetLastError();
                        isProcessRunning = false;
                    }
                }
            }
            break;

        case 4:
            {
            MessageBox(hWnd, _T("Deposit button clicked!"), _T("Button Click"), MB_OK);
            string content = fileToString("C:\\Facultate\\CSSO\\Week4\\Reports\\Summary\\errors.txt", 100*10000); 
            TCHAR* buffer = new TCHAR[content.size() + 1];
            _tcscpy_s(buffer, content.size() + 1, content.c_str());
            SetWindowText(outputEdit, buffer);
            delete[] buffer;

            break;    
            }

        default:
            break;
        }
        break;

    case WM_KEYDOWN:
        if (GetFocus() == editBox && wParam == VK_RETURN)
        {
            int len = GetWindowTextLength(editBox);
            if (len > 0)
            {
                TCHAR* buffer = new TCHAR[len + 1];
                GetWindowText(editBox, buffer, len + 1);
                MessageBox(hWnd, buffer, _T("Input"), MB_OK);
                _tcscpy_s(greeting, len + 1, buffer);
                InvalidateRect(hWnd, NULL, TRUE);
                std::wcout << buffer;
                delete[] buffer;
            }
            else
            {
                MessageBox(hWnd, _T("No data was provided"), _T("ERROR"), MB_ICONERROR);
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_CLOSE:
        // Add any cleanup or confirmation dialogs here
        PostQuitMessage(0);
        break;
    

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}
