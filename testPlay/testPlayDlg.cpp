
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



CtestPlayDlg::CtestPlayDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CtestPlayDlg::IDD, pParent)
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
END_MESSAGE_MAP()


// CtestPlayDlg message handlers

BOOL CtestPlayDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    // TODO: Add extra initialization here
    m_hDll = LoadLibrary(L"RTSPFFmpeg.dll");
    if(NULL == m_hDll)
    {
        AfxMessageBox(L"Load RTSPFFmpeg.dll error");
    }

    InitRtspDLL = (lpFuncInitRtspDLL)GetProcAddress(m_hDll, "InitRtspDLL");
    FreeRtspDll = (lpFuncFreeRtspDll)GetProcAddress(m_hDll, "FreeRtspDLL");
    PlayRtsp = (lpFuncPlayRtsp)GetProcAddress(m_hDll, "PlayRtsp");
    StopRtsp = (lpFuncStopRtsp)GetProcAddress(m_hDll, "StopRtsp");
    InitRtspVideoParam = (lpFuncInitRtspVideoParam)GetProcAddress(m_hDll, "InitRtspVideoParam");
    RevoHWAcceleration = (lpFuncRevoHWAcceleration)GetProcAddress(m_hDll, "RevoHWAcceleration");
    GetRtspINSTANCE = (lpFuncGetRtspINSTANCE)GetProcAddress(m_hDll, "GetRtspINSTANCE");

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
    char URI[] = "rtsp://192.168.10.141:554/Streaming/Channels/1?transportmode=unicast&profile=Profile_1";

    if(-1 == InitRtspDLL())
    {
        AfxMessageBox(L"InitRtspDLL error");
    }

    int INSTANCE = GetRtspINSTANCE();
    InitRtspVideoParam(INSTANCE, URI, "admin", "12345");
    PlayRtsp(INSTANCE, GetDlgItem(IDC_PICTURE_AREA)->m_hWnd);
}

BOOL CtestPlayDlg::DestroyWindow()
{
    // TODO: Add your specialized code here and/or call the base class

    if(-1 == FreeRtspDll())
    {
        AfxMessageBox(L"FreeRtspDll error");
    }

    FreeLibrary(m_hDll);

    return CDialogEx::DestroyWindow();
}
