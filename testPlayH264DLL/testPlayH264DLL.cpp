// testPlayH264DLL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "testPlayH264DLL.h"
#include <PlayH264DLL.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
TCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
int playing_intance;
HWND hWnd;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI DecodeThread(LPVOID lpParam)
{
    FILE* pDataFile = fopen("test_video_data.h264", "rb");
    FILE* pSizeFile = fopen("test_video_size", "rb");

    fseek(pDataFile, 0, SEEK_END);
    fseek(pSizeFile, 0, SEEK_END);

    int test_video_data_file_size = ftell(pDataFile);
    int test_video_size_file_size = ftell(pSizeFile);

    rewind(pDataFile);
    rewind(pSizeFile);

    unsigned int* size = new unsigned int[test_video_size_file_size / 4];
    char* buffer = new char[test_video_data_file_size];

    fread(size, test_video_size_file_size, 1, pSizeFile);
    fread(buffer, test_video_data_file_size, 1, pDataFile);

    fclose(pSizeFile);
    fclose(pDataFile);

    unsigned int CurrentPos = 0;
    unsigned int DataJumpBytes = 0;

    while((*(bool*)lpParam))
    {
        decode(playing_intance, (unsigned char*)(buffer + DataJumpBytes), size[CurrentPos], 0, 0);

        DataJumpBytes += size[CurrentPos];
        CurrentPos++;
        if(CurrentPos >= test_video_size_file_size / 4)
        {
            CurrentPos = 0;
            DataJumpBytes = 0;
        }
    }

    delete[] buffer;
    delete[] size;

    return 0;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
    playing_intance = -1;

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    MSG msg;
    HACCEL hAccelTable;

    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_TESTPLAYH264DLL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if(!InitInstance(hInstance, nCmdShow) || 0 != initial_decode_DLL(10))
    {
        return FALSE;
    }

    playing_intance = get_idle_decode_instance();

    myparamInput input_parameter;
    memset(&input_parameter, 0x0, sizeof(myparamInput));
    input_parameter.fps = 25;
    input_parameter.height = 720;
    input_parameter.isDecode = true;
    input_parameter.playChannle = 1;
    input_parameter.playHandle = hWnd;
    input_parameter.width = 1280;

    //set_decode_hardware_acceleration(playing_intance, true);

    initial_decode_parameter(playing_intance, &input_parameter, 1);

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTPLAYH264DLL));

    DWORD dDecodeThreadID;
    HANDLE hDecodeThread;
    bool bLoop = true;

    hDecodeThread = CreateThread(NULL, 0, DecodeThread, &bLoop, 0, &dDecodeThreadID);

    // Main message loop:
    while(GetMessage(&msg, NULL, 0, 0))
    {
        if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    bLoop = false;

    WaitForSingleObject(hDecodeThread, INFINITE);

    free_decode_DLL();

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTPLAYH264DLL));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDC_TESTPLAYH264DLL);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if(!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    switch(message)
    {
        case WM_COMMAND:
            wmId = LOWORD(wParam);
            wmEvent = HIWORD(wParam);
            // Parse the menu selections:
            switch(wmId)
            {
                case IDM_ABOUT:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    break;
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code here...
            EndPaint(hWnd, &ps);
            break;
            //case WM_SIZE:
            //    playing_windows_RECT_changed_of_decode_DLL(playing_intance);
            //    break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch(message)
    {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}
