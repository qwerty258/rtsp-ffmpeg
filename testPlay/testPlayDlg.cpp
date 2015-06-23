
// testPlayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testPlay.h"
#include "testPlayDlg.h"
#include "afxdialogex.h"

#include <cstdio>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestPlayDlg dialog



CtestPlayDlg::CtestPlayDlg(CWnd* pParent /*=NULL*/): CDialogEx(CtestPlayDlg::IDD, pParent), m_instance_1(0)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_rect_change = TRUE;
    m_instance_1 = -1;
    m_instance_2 = -1;
}

void CtestPlayDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestPlayDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_PLAY, &CtestPlayDlg::OnClickedButtonPlay)
    ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CtestPlayDlg::OnClickedButtonPause)
    ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CtestPlayDlg::OnClickedButtonConnect)
    ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CtestPlayDlg::OnClickedButtonDisconnect)
    ON_BN_CLICKED(IDC_BUTTON_CONNECT_2, &CtestPlayDlg::OnClickedButtonConnect2)
    ON_BN_CLICKED(IDC_BUTTON_PALY_2, &CtestPlayDlg::OnClickedButtonPaly2)
    ON_BN_CLICKED(IDC_BUTTON_PAUSE_2, &CtestPlayDlg::OnClickedButtonPause2)
    ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_2, &CtestPlayDlg::OnClickedButtonDisconnect2)
    ON_BN_CLICKED(IDC_BUTTON_RESIZE, &CtestPlayDlg::OnClickedButtonResize)
END_MESSAGE_MAP()


// CtestPlayDlg message handlers

BOOL CtestPlayDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);  // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon

    // TODO: Add extra initialization here
    if(0 != initial_RTSP_DLL(20))
    {
        AfxMessageBox(L"InitRtspDLL error");
    }

    //char* buffer = new char[256];

    //if(SDL_Init(SDL_INIT_VIDEO))
    //{
    //    sprintf(buffer, "Could not initialize SDL - %s\n", SDL_GetError());
    //    MessageBoxA(NULL, buffer, NULL, MB_OK);
    //}

    //screen = SDL_CreateWindow("YUV420p callback test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_OPENGL);
    //if(!screen)
    //{
    //    sprintf(buffer, "SDL: could not create window - exiting:%s\n", SDL_GetError());
    //    MessageBoxA(NULL, buffer, NULL, MB_OK);
    //}

    //sdlRenderer = SDL_CreateRenderer(screen, -1, 0);

    //delete[] buffer;

    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CtestPlayDlg::OnPaint()
{
    if(IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CtestPlayDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

typedef int(*function_resize)(int instance, int width, int height);

void CtestPlayDlg::OnClickedButtonPlay()
{
    // TODO: Add your control notification handler code here
    if(0 > play(m_instance_1))
    {
        AfxMessageBox(L"Play error");
    }
}

BOOL CtestPlayDlg::DestroyWindow()
{
    // TODO: Add your specialized code here and/or call the base class
    if(0 != free_RTSP_DLL())
    {
        AfxMessageBox(L"FreeRtspDll error");
    }

    return CDialogEx::DestroyWindow();
}


void CtestPlayDlg::OnClickedButtonPause()
{
    // TODO: Add your control notification handler code here
    if(0 != pause(m_instance_1))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnClickedButtonConnect()
{
    // TODO: Add your control notification handler code here
    char URI[] = "rtsp://192.168.10.195:554/Streaming/Channels/1?transportmode=unicast&profile=Profile_1";

    m_instance_1 = get_idle_instance();

    set_hardware_acceleration(m_instance_1, false);
    int i = 1;
    set_H264_callback(m_instance_1, H264_callback, (void*)i, true);
    set_YUV420_callback(m_instance_1, YUV420_callback, (void*)i, true);
    set_YV12_callback(m_instance_1, YV12_callback, (void*)i, true);

    if(0 > m_instance_1)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_1, URI, "admin", "12345", GetDlgItem(IDC_PICTURE_AREA)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_1))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnClickedButtonDisconnect()
{
    // TODO: Add your control notification handler code here
    if(0 > RTSP_disconnect(m_instance_1))
    {
        AfxMessageBox(L"DisConnect error");
    }
}

int H264_callback(int instance, char* frame_buff, int frame_buffer_size, int frame_width, int frame_height, void* userdata, int frame_lost)
{
    FILE* pFile = fopen("C:\\h264_callback.log", "ab");
    char* buffer = new char[2048];

    sprintf(buffer, "instance: %02d, frame_buff: %p,frame_buffer_size: %08d, frame_width: %d, frame_height: %d, userdata: %p, frame_lost:%02d\n", instance, frame_buff, frame_buffer_size, frame_width, frame_height, userdata, frame_lost);

    fwrite(buffer, 1, strlen(buffer), pFile);

    delete[] buffer;
    fclose(pFile);

    return 0;
}

int YUV420_callback(int instance, char* frame_buffer, int frame_buffer_size, int frame_width, int frame_height, size_t frame_ID, void* userdata, int frame_lost)
{
    FILE* pFile = fopen("C:\\YUV420_callback.log", "ab");
    char* buffer = new char[2048];

    sprintf(buffer, "instance: %02d, frame_buff: %p,frame_buffer_size: %08d, frame_width: %d, frame_height: %d, userdata: %p, frame_lost:%02d\n", instance, frame_buffer, frame_buffer_size, frame_width, frame_height, userdata, frame_lost);

    fwrite(buffer, 1, strlen(buffer), pFile);

    delete[] buffer;
    fclose(pFile);

    return 0;
}

int YV12_callback(int instance, char* frame_buff, int frame_buffer_size, int frame_width, int frame_height, void* userdata, int frame_lost)
{
    FILE* pFile = fopen("C:\\YV12_callback.log", "ab");
    char* buffer = new char[2048];

    sprintf(buffer, "instance: %02d, frame_buff: %p,frame_buffer_size: %08d, frame_width: %d, frame_height: %d, userdata: %p, frame_lost:%02d\n", instance, frame_buff, frame_buffer_size, frame_width, frame_height, userdata, frame_lost);

    fwrite(buffer, 1, strlen(buffer), pFile);

    delete[] buffer;
    fclose(pFile);

    return 0;

}

void CtestPlayDlg::OnClickedButtonConnect2()
{
    // TODO: Add your control notification handler code here
    char URI[] = "rtsp://192.168.10.185:554/Streaming/Channels/1?transportmode=unicast&profile=Profile_1";

    m_instance_2 = get_idle_instance();

    set_hardware_acceleration(m_instance_2, false);

    if(0 > m_instance_2)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_2, URI, "admin", "12345", GetDlgItem(IDC_PICTURE_AREA_2)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_2))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnClickedButtonPaly2()
{
    // TODO: Add your control notification handler code here
    if(0 > play(m_instance_2))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnClickedButtonPause2()
{
    // TODO: Add your control notification handler code here
    if(0 != pause(m_instance_2))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnClickedButtonDisconnect2()
{
    // TODO: Add your control notification handler code here
    if(0 > RTSP_disconnect(m_instance_2))
    {
        AfxMessageBox(L"DisConnect error");
    }
}


void CtestPlayDlg::OnClickedButtonResize()
{
    // TODO: Add your control notification handler code here
    if(m_rect_change)
    {
        GetDlgItem(IDC_PICTURE_AREA)->SetWindowPos(NULL, 0, 0, 300, 300, SWP_NOZORDER | SWP_NOMOVE);
    }
    else
    {
        GetDlgItem(IDC_PICTURE_AREA)->SetWindowPos(NULL, 0, 0, 600, 400, SWP_NOZORDER | SWP_NOMOVE);
    }

    playing_windows_RECT_changed(m_instance_1);

    m_rect_change = !m_rect_change;
}
