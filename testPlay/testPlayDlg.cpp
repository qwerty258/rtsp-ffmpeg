
// testPlayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testPlay.h"
#include "testPlayDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestPlayDlg dialog



CtestPlayDlg::CtestPlayDlg(CWnd* pParent /*=NULL*/): CDialogEx(CtestPlayDlg::IDD, pParent), m_INSTANCE(0)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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

void CtestPlayDlg::OnClickedButtonPlay()
{
    // TODO: Add your control notification handler code here
    set_hardware_acceleration(m_INSTANCE, false);
    set_YUV420_callback(m_INSTANCE, NULL, NULL, true);
    if(0 > play(m_INSTANCE))
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
    if(0 != pause(m_INSTANCE))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnClickedButtonConnect()
{
    // TODO: Add your control notification handler code here
    char URI[] = "rtsp://192.168.10.185:554/Streaming/Channels/1?transportmode=unicast&profile=Profile_1";

    m_INSTANCE = get_idle_instance();
    if(0 > m_INSTANCE)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_INSTANCE, URI, "admin", "12345", GetDlgItem(IDC_PICTURE_AREA)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_INSTANCE))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnClickedButtonDisconnect()
{
    // TODO: Add your control notification handler code here
    if(0 > RTSP_disconnect(m_INSTANCE))
    {
        AfxMessageBox(L"DisConnect error");
    }
}
