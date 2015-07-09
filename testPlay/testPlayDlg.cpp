
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



CtestPlayDlg::CtestPlayDlg(CWnd* pParent /*=NULL*/): CDialogEx(CtestPlayDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_rect_change = TRUE;
    m_instance_1 = -1;
    m_instance_2 = -1;
    m_instance_3 = -1;
    m_instance_4 = -1;
    m_instance_5 = -1;
    m_instance_6 = -1;
    m_instance_7 = -1;
    m_instance_8 = -1;
    m_instance_9 = -1;
    m_instance_10 = -1;
    m_instance_11 = -1;
    m_instance_12 = -1;
    m_instance_13 = -1;
    m_instance_14 = -1;
    m_instance_15 = -1;
    m_instance_16 = -1;
    m_instance_17 = -1;
    m_instance_18 = -1;
    m_instance_19 = -1;
    m_instance_20 = -1;
    m_instance_21 = -1;
    m_instance_22 = -1;
    m_instance_23 = -1;
    m_instance_24 = -1;
    m_instance_25 = -1;
    m_instance_26 = -1;
    m_instance_27 = -1;
    m_instance_28 = -1;
    m_instance_29 = -1;
    m_instance_30 = -1;
    m_instance_31 = -1;
    m_instance_32 = -1;
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
    ON_BN_CLICKED(IDC_BUTTON_DESKTOP, &CtestPlayDlg::OnClickedButtonDesktop)
    ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION, &CtestPlayDlg::OnClickedButtonHwacceleration)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION2, &CtestPlayDlg::OnBnClickedButtonHwacceleration2)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_3, &CtestPlayDlg::OnBnClickedButtonConnect3)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION3, &CtestPlayDlg::OnBnClickedButtonHwacceleration3)
ON_BN_CLICKED(IDC_BUTTON_PALY_3, &CtestPlayDlg::OnBnClickedButtonPaly3)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_3, &CtestPlayDlg::OnBnClickedButtonPause3)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_3, &CtestPlayDlg::OnBnClickedButtonDisconnect3)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_4, &CtestPlayDlg::OnBnClickedButtonConnect4)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION4, &CtestPlayDlg::OnBnClickedButtonHwacceleration4)
ON_BN_CLICKED(IDC_BUTTON_PALY_4, &CtestPlayDlg::OnBnClickedButtonPaly4)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_4, &CtestPlayDlg::OnBnClickedButtonPause4)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_4, &CtestPlayDlg::OnBnClickedButtonDisconnect4)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_5, &CtestPlayDlg::OnBnClickedButtonConnect5)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION5, &CtestPlayDlg::OnBnClickedButtonHwacceleration5)
ON_BN_CLICKED(IDC_BUTTON_PALY_5, &CtestPlayDlg::OnBnClickedButtonPaly5)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_5, &CtestPlayDlg::OnBnClickedButtonPause5)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_5, &CtestPlayDlg::OnBnClickedButtonDisconnect5)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_6, &CtestPlayDlg::OnBnClickedButtonConnect6)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION6, &CtestPlayDlg::OnBnClickedButtonHwacceleration6)
ON_BN_CLICKED(IDC_BUTTON_PALY_6, &CtestPlayDlg::OnBnClickedButtonPaly6)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_6, &CtestPlayDlg::OnBnClickedButtonPause6)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_6, &CtestPlayDlg::OnBnClickedButtonDisconnect6)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_7, &CtestPlayDlg::OnBnClickedButtonConnect7)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION7, &CtestPlayDlg::OnBnClickedButtonHwacceleration7)
ON_BN_CLICKED(IDC_BUTTON_PALY_7, &CtestPlayDlg::OnBnClickedButtonPaly7)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_7, &CtestPlayDlg::OnBnClickedButtonPause7)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_7, &CtestPlayDlg::OnBnClickedButtonDisconnect7)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_29, &CtestPlayDlg::OnBnClickedButtonConnect29)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION29, &CtestPlayDlg::OnBnClickedButtonHwacceleration29)
ON_BN_CLICKED(IDC_BUTTON_PALY_29, &CtestPlayDlg::OnBnClickedButtonPaly29)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_29, &CtestPlayDlg::OnBnClickedButtonPause29)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_29, &CtestPlayDlg::OnBnClickedButtonDisconnect29)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_8, &CtestPlayDlg::OnBnClickedButtonConnect8)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION8, &CtestPlayDlg::OnBnClickedButtonHwacceleration8)
ON_BN_CLICKED(IDC_BUTTON_PALY_8, &CtestPlayDlg::OnBnClickedButtonPaly8)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_8, &CtestPlayDlg::OnBnClickedButtonPause8)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_8, &CtestPlayDlg::OnBnClickedButtonDisconnect8)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_9, &CtestPlayDlg::OnBnClickedButtonConnect9)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION9, &CtestPlayDlg::OnBnClickedButtonHwacceleration9)
ON_BN_CLICKED(IDC_BUTTON_PALY_9, &CtestPlayDlg::OnBnClickedButtonPaly9)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_9, &CtestPlayDlg::OnBnClickedButtonPause9)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_9, &CtestPlayDlg::OnBnClickedButtonDisconnect9)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_10, &CtestPlayDlg::OnBnClickedButtonConnect10)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION10, &CtestPlayDlg::OnBnClickedButtonHwacceleration10)
ON_BN_CLICKED(IDC_BUTTON_PALY_10, &CtestPlayDlg::OnBnClickedButtonPaly10)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_10, &CtestPlayDlg::OnBnClickedButtonPause10)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_10, &CtestPlayDlg::OnBnClickedButtonDisconnect10)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_11, &CtestPlayDlg::OnBnClickedButtonConnect11)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION11, &CtestPlayDlg::OnBnClickedButtonHwacceleration11)
ON_BN_CLICKED(IDC_BUTTON_PALY_11, &CtestPlayDlg::OnBnClickedButtonPaly11)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_11, &CtestPlayDlg::OnBnClickedButtonPause11)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_11, &CtestPlayDlg::OnBnClickedButtonDisconnect11)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_12, &CtestPlayDlg::OnBnClickedButtonConnect12)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION12, &CtestPlayDlg::OnBnClickedButtonHwacceleration12)
ON_BN_CLICKED(IDC_BUTTON_PALY_12, &CtestPlayDlg::OnBnClickedButtonPaly12)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_12, &CtestPlayDlg::OnBnClickedButtonPause12)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_12, &CtestPlayDlg::OnBnClickedButtonDisconnect12)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_13, &CtestPlayDlg::OnBnClickedButtonConnect13)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION13, &CtestPlayDlg::OnBnClickedButtonHwacceleration13)
ON_BN_CLICKED(IDC_BUTTON_PALY_13, &CtestPlayDlg::OnBnClickedButtonPaly13)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_13, &CtestPlayDlg::OnBnClickedButtonPause13)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_13, &CtestPlayDlg::OnBnClickedButtonDisconnect13)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_14, &CtestPlayDlg::OnBnClickedButtonConnect14)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION14, &CtestPlayDlg::OnBnClickedButtonHwacceleration14)
ON_BN_CLICKED(IDC_BUTTON_PALY_14, &CtestPlayDlg::OnBnClickedButtonPaly14)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_14, &CtestPlayDlg::OnBnClickedButtonPause14)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_14, &CtestPlayDlg::OnBnClickedButtonDisconnect14)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_30, &CtestPlayDlg::OnBnClickedButtonConnect30)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION30, &CtestPlayDlg::OnBnClickedButtonHwacceleration30)
ON_BN_CLICKED(IDC_BUTTON_PALY_30, &CtestPlayDlg::OnBnClickedButtonPaly30)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_30, &CtestPlayDlg::OnBnClickedButtonPause30)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_30, &CtestPlayDlg::OnBnClickedButtonDisconnect30)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_15, &CtestPlayDlg::OnBnClickedButtonConnect15)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION15, &CtestPlayDlg::OnBnClickedButtonHwacceleration15)
ON_BN_CLICKED(IDC_BUTTON_PALY_15, &CtestPlayDlg::OnBnClickedButtonPaly15)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_15, &CtestPlayDlg::OnBnClickedButtonPause15)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_15, &CtestPlayDlg::OnBnClickedButtonDisconnect15)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_16, &CtestPlayDlg::OnBnClickedButtonConnect16)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION16, &CtestPlayDlg::OnBnClickedButtonHwacceleration16)
ON_BN_CLICKED(IDC_BUTTON_PALY_16, &CtestPlayDlg::OnBnClickedButtonPaly16)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_16, &CtestPlayDlg::OnBnClickedButtonPause16)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_16, &CtestPlayDlg::OnBnClickedButtonDisconnect16)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_17, &CtestPlayDlg::OnBnClickedButtonConnect17)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION17, &CtestPlayDlg::OnBnClickedButtonHwacceleration17)
ON_BN_CLICKED(IDC_BUTTON_PALY_17, &CtestPlayDlg::OnBnClickedButtonPaly17)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_17, &CtestPlayDlg::OnBnClickedButtonPause17)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_17, &CtestPlayDlg::OnBnClickedButtonDisconnect17)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_18, &CtestPlayDlg::OnBnClickedButtonConnect18)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION18, &CtestPlayDlg::OnBnClickedButtonHwacceleration18)
ON_BN_CLICKED(IDC_BUTTON_PALY_18, &CtestPlayDlg::OnBnClickedButtonPaly18)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_18, &CtestPlayDlg::OnBnClickedButtonPause18)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_18, &CtestPlayDlg::OnBnClickedButtonDisconnect18)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_19, &CtestPlayDlg::OnBnClickedButtonConnect19)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION19, &CtestPlayDlg::OnBnClickedButtonHwacceleration19)
ON_BN_CLICKED(IDC_BUTTON_PALY_19, &CtestPlayDlg::OnBnClickedButtonPaly19)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_19, &CtestPlayDlg::OnBnClickedButtonPause19)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_19, &CtestPlayDlg::OnBnClickedButtonDisconnect19)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_20, &CtestPlayDlg::OnBnClickedButtonConnect20)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION20, &CtestPlayDlg::OnBnClickedButtonHwacceleration20)
ON_BN_CLICKED(IDC_BUTTON_PALY_20, &CtestPlayDlg::OnBnClickedButtonPaly20)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_20, &CtestPlayDlg::OnBnClickedButtonPause20)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_20, &CtestPlayDlg::OnBnClickedButtonDisconnect20)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_21, &CtestPlayDlg::OnBnClickedButtonConnect21)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION21, &CtestPlayDlg::OnBnClickedButtonHwacceleration21)
ON_BN_CLICKED(IDC_BUTTON_PALY_21, &CtestPlayDlg::OnBnClickedButtonPaly21)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_21, &CtestPlayDlg::OnBnClickedButtonPause21)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_21, &CtestPlayDlg::OnBnClickedButtonDisconnect21)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_31, &CtestPlayDlg::OnBnClickedButtonConnect31)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION31, &CtestPlayDlg::OnBnClickedButtonHwacceleration31)
ON_BN_CLICKED(IDC_BUTTON_PALY_31, &CtestPlayDlg::OnBnClickedButtonPaly31)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_31, &CtestPlayDlg::OnBnClickedButtonPause31)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_31, &CtestPlayDlg::OnBnClickedButtonDisconnect31)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_22, &CtestPlayDlg::OnBnClickedButtonConnect22)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION22, &CtestPlayDlg::OnBnClickedButtonHwacceleration22)
ON_BN_CLICKED(IDC_BUTTON_PALY_22, &CtestPlayDlg::OnBnClickedButtonPaly22)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_22, &CtestPlayDlg::OnBnClickedButtonPause22)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_22, &CtestPlayDlg::OnBnClickedButtonDisconnect22)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_23, &CtestPlayDlg::OnBnClickedButtonConnect23)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION23, &CtestPlayDlg::OnBnClickedButtonHwacceleration23)
ON_BN_CLICKED(IDC_BUTTON_PALY_23, &CtestPlayDlg::OnBnClickedButtonPaly23)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_23, &CtestPlayDlg::OnBnClickedButtonPause23)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_23, &CtestPlayDlg::OnBnClickedButtonDisconnect23)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_24, &CtestPlayDlg::OnBnClickedButtonConnect24)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION24, &CtestPlayDlg::OnBnClickedButtonHwacceleration24)
ON_BN_CLICKED(IDC_BUTTON_PALY_24, &CtestPlayDlg::OnBnClickedButtonPaly24)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_24, &CtestPlayDlg::OnBnClickedButtonPause24)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_24, &CtestPlayDlg::OnBnClickedButtonDisconnect24)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_25, &CtestPlayDlg::OnBnClickedButtonConnect25)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION25, &CtestPlayDlg::OnBnClickedButtonHwacceleration25)
ON_BN_CLICKED(IDC_BUTTON_PALY_25, &CtestPlayDlg::OnBnClickedButtonPaly25)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_25, &CtestPlayDlg::OnBnClickedButtonPause25)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_25, &CtestPlayDlg::OnBnClickedButtonDisconnect25)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_26, &CtestPlayDlg::OnBnClickedButtonConnect26)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION26, &CtestPlayDlg::OnBnClickedButtonHwacceleration26)
ON_BN_CLICKED(IDC_BUTTON_PALY_26, &CtestPlayDlg::OnBnClickedButtonPaly26)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_26, &CtestPlayDlg::OnBnClickedButtonPause26)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_26, &CtestPlayDlg::OnBnClickedButtonDisconnect26)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_27, &CtestPlayDlg::OnBnClickedButtonConnect27)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_28, &CtestPlayDlg::OnBnClickedButtonConnect28)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION27, &CtestPlayDlg::OnBnClickedButtonHwacceleration27)
ON_BN_CLICKED(IDC_BUTTON_PALY_27, &CtestPlayDlg::OnBnClickedButtonPaly27)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_27, &CtestPlayDlg::OnBnClickedButtonPause27)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_27, &CtestPlayDlg::OnBnClickedButtonDisconnect27)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION28, &CtestPlayDlg::OnBnClickedButtonHwacceleration28)
ON_BN_CLICKED(IDC_BUTTON_PALY_28, &CtestPlayDlg::OnBnClickedButtonPaly28)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_28, &CtestPlayDlg::OnBnClickedButtonPause28)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_28, &CtestPlayDlg::OnBnClickedButtonDisconnect28)
ON_BN_CLICKED(IDC_BUTTON_CONNECT_32, &CtestPlayDlg::OnBnClickedButtonConnect32)
ON_BN_CLICKED(IDC_BUTTON_HWACCELERATION32, &CtestPlayDlg::OnBnClickedButtonHwacceleration32)
ON_BN_CLICKED(IDC_BUTTON_PALY_32, &CtestPlayDlg::OnBnClickedButtonPaly32)
ON_BN_CLICKED(IDC_BUTTON_PAUSE_32, &CtestPlayDlg::OnBnClickedButtonPause32)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_32, &CtestPlayDlg::OnBnClickedButtonDisconnect32)
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
    if(0 != initial_RTSP_DLL(40))
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

BOOL CtestPlayDlg::DestroyWindow()
{
    // TODO: Add your specialized code here and/or call the base class
    if(0 != free_RTSP_DLL())
    {
        AfxMessageBox(L"FreeRtspDll error");
    }

    return CDialogEx::DestroyWindow();
}

int H264_callback(int instance, char* frame_buff, int frame_buffer_size, int frame_width, int frame_height, void* userdata, int frame_lost)
{
    FILE* pFile = fopen("C:\\h264_callback.log", "ab");
    FILE* pDataFile = fopen("C:\\test_video_data.h264", "ab");
    FILE* pSizeFile = fopen("C:\\test_video_size", "ab");
    char* buffer = new char[2048];

    sprintf(buffer, "instance: %02d, frame_buff: %p,frame_buffer_size: %08d, frame_width: %d, frame_height: %d, userdata: %p, frame_lost:%02d\n", instance, frame_buff, frame_buffer_size, frame_width, frame_height, userdata, frame_lost);

    fwrite(buffer, 1, strlen(buffer), pFile);

    fwrite(&frame_buffer_size, 4, 1, pSizeFile);
    fwrite(frame_buff, frame_buffer_size, 1, pDataFile);

    delete[] buffer;
    fclose(pFile);
    fclose(pDataFile);
    fclose(pSizeFile);

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

void CtestPlayDlg::OnClickedButtonConnect()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT1)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT33)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT34)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_1 = get_idle_instance();

    int i = 1;
    set_H264_callback(m_instance_1, H264_callback, (void*)i, true);
    //set_YUV420_callback(m_instance_1, YUV420_callback, (void*)i, true);
    //set_YV12_callback(m_instance_1, YV12_callback, (void*)i, true);

    if(0 > m_instance_1)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_1, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_1))
    {
        AfxMessageBox(L"Connect error");
    }
}

void CtestPlayDlg::OnClickedButtonHwacceleration()
{
    set_hardware_acceleration(m_instance_1, true);
}

void CtestPlayDlg::OnClickedButtonPlay()
{
    if(0 > play(m_instance_1))
    {
        AfxMessageBox(L"Play error");
    }
}

void CtestPlayDlg::OnClickedButtonPause()
{
    if(0 != pause(m_instance_1))
    {
        AfxMessageBox(L"Pause error");
    }
}

void CtestPlayDlg::OnClickedButtonDisconnect()
{
    if(0 > RTSP_disconnect(m_instance_1))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_1 = -1;
}

void CtestPlayDlg::OnClickedButtonResize()
{
    if(m_rect_change)
    {
        GetDlgItem(IDC_PICTURE_AREA)->SetWindowPos(NULL, 0, 0, 1280, 720, SWP_NOZORDER | SWP_NOMOVE);
    }
    else
    {
        GetDlgItem(IDC_PICTURE_AREA)->SetWindowPos(NULL, 0, 0, 216, 146, SWP_NOZORDER | SWP_NOMOVE);
    }

    playing_windows_RECT_changed(m_instance_1);

    m_rect_change = !m_rect_change;
}


void CtestPlayDlg::OnClickedButtonDesktop()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT1)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT33)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT34)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_1 = get_idle_instance();

    if(0 > m_instance_1)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_1, strURI, user, pwd, ::GetDesktopWindow()))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_1))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnClickedButtonConnect2()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT2)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT35)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT36)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_2 = get_idle_instance();

    if(0 > m_instance_2)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_2, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_2)->m_hWnd))
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
    if(0 > play(m_instance_2))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnClickedButtonPause2()
{
    if(0 != pause(m_instance_2))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnClickedButtonDisconnect2()
{
    if(0 > RTSP_disconnect(m_instance_2))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_2 = -1;
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration2()
{
    set_hardware_acceleration(m_instance_2, true);
}


void CtestPlayDlg::OnBnClickedButtonConnect3()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT3)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT37)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT38)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_3 = get_idle_instance();

    if(0 > m_instance_3)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_3, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_3)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_3))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration3()
{
    set_hardware_acceleration(m_instance_3, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly3()
{
    if(0 > play(m_instance_3))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause3()
{
    if(0 != pause(m_instance_3))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect3()
{
    if(0 > RTSP_disconnect(m_instance_3))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_3 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect4()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT4)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT39)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT40)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_4 = get_idle_instance();

    if(0 > m_instance_4)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_4, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_4)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_4))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration4()
{
    set_hardware_acceleration(m_instance_4, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly4()
{
    if(0 > play(m_instance_4))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause4()
{
    if(0 != pause(m_instance_4))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect4()
{
    if(0 > RTSP_disconnect(m_instance_4))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_4 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect5()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT5)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT41)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT42)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_5 = get_idle_instance();

    if(0 > m_instance_5)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_5, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_5)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_5))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration5()
{
    set_hardware_acceleration(m_instance_5, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly5()
{
    if(0 > play(m_instance_5))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause5()
{
    if(0 != pause(m_instance_5))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect5()
{
    if(0 > RTSP_disconnect(m_instance_5))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_5 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect6()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT6)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT43)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT44)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_6 = get_idle_instance();

    if(0 > m_instance_6)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_6, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_6)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_6))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration6()
{
    set_hardware_acceleration(m_instance_6, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly6()
{
    if(0 > play(m_instance_6))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause6()
{
    if(0 != pause(m_instance_6))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect6()
{
    if(0 > RTSP_disconnect(m_instance_6))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_6 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect7()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT7)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT45)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT46)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_7 = get_idle_instance();

    if(0 > m_instance_7)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_7, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_7)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_7))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration7()
{
    set_hardware_acceleration(m_instance_7, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly7()
{
    if(0 > play(m_instance_7))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause7()
{
    if(0 != pause(m_instance_7))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect7()
{
    if(0 > RTSP_disconnect(m_instance_7))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_7 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect29()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT29)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT47)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT48)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_8 = get_idle_instance();

    if(0 > m_instance_8)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_8, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_29)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_8))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration29()
{
    set_hardware_acceleration(m_instance_8, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly29()
{
    if(0 > play(m_instance_8))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause29()
{
    if(0 != pause(m_instance_8))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect29()
{
    if(0 > RTSP_disconnect(m_instance_8))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_8 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect8()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT8)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT49)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT50)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_9 = get_idle_instance();

    if(0 > m_instance_9)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_9, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_8)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_9))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration8()
{
    set_hardware_acceleration(m_instance_9, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly8()
{
    if(0 > play(m_instance_9))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause8()
{
    if(0 != pause(m_instance_9))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect8()
{
    if(0 > RTSP_disconnect(m_instance_9))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_9 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect9()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT9)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT51)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT52)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_10 = get_idle_instance();

    if(0 > m_instance_10)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_10, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_9)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_10))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration9()
{
    set_hardware_acceleration(m_instance_10, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly9()
{
    if(0 > play(m_instance_10))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause9()
{
    if(0 != pause(m_instance_10))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect9()
{
    if(0 > RTSP_disconnect(m_instance_10))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_10 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect10()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT10)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT53)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT54)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_11 = get_idle_instance();

    if(0 > m_instance_11)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_11, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_10)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_11))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration10()
{
    set_hardware_acceleration(m_instance_11, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly10()
{
    if(0 > play(m_instance_11))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause10()
{
    if(0 != pause(m_instance_11))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect10()
{
    if(0 > RTSP_disconnect(m_instance_11))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_11 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect11()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT11)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT55)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT56)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_12 = get_idle_instance();

    if(0 > m_instance_12)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_12, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_11)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_12))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration11()
{
    set_hardware_acceleration(m_instance_12, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly11()
{
    if(0 > play(m_instance_12))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause11()
{
    if(0 != pause(m_instance_12))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect11()
{
    if(0 > RTSP_disconnect(m_instance_12))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_12 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect12()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT12)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT57)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT58)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_13 = get_idle_instance();

    if(0 > m_instance_13)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_13, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_12)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_13))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration12()
{
    set_hardware_acceleration(m_instance_13, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly12()
{
    if(0 > play(m_instance_13))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause12()
{
    if(0 != pause(m_instance_13))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect12()
{
    if(0 > RTSP_disconnect(m_instance_13))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_13 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect13()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT13)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT59)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT60)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_14 = get_idle_instance();

    if(0 > m_instance_14)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_14, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_13)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_14))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration13()
{
    set_hardware_acceleration(m_instance_14, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly13()
{
    if(0 > play(m_instance_14))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause13()
{
    if(0 != pause(m_instance_14))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect13()
{
    if(0 > RTSP_disconnect(m_instance_14))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_14 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect14()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT14)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT61)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT62)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_15 = get_idle_instance();

    if(0 > m_instance_15)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_15, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_14)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_15))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration14()
{
    set_hardware_acceleration(m_instance_15, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly14()
{
    if(0 > play(m_instance_15))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause14()
{
    if(0 != pause(m_instance_15))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect14()
{
    if(0 > RTSP_disconnect(m_instance_15))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_15 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect30()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT30)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT63)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT64)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_16 = get_idle_instance();

    if(0 > m_instance_16)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_16, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_30)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_16))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration30()
{
    set_hardware_acceleration(m_instance_16, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly30()
{
    if(0 > play(m_instance_16))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause30()
{
    if(0 != pause(m_instance_16))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect30()
{
    if(0 > RTSP_disconnect(m_instance_16))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_16 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect15()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT15)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT65)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT66)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_17 = get_idle_instance();

    if(0 > m_instance_17)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_17, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_15)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_17))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration15()
{
    set_hardware_acceleration(m_instance_17, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly15()
{
    if(0 > play(m_instance_17))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause15()
{
    if(0 != pause(m_instance_17))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect15()
{
    if(0 > RTSP_disconnect(m_instance_17))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_17 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect16()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT16)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT67)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT68)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_18 = get_idle_instance();

    if(0 > m_instance_18)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_18, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_16)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_18))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration16()
{
    set_hardware_acceleration(m_instance_18, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly16()
{
    if(0 > play(m_instance_18))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause16()
{
    if(0 != pause(m_instance_18))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect16()
{
    if(0 > RTSP_disconnect(m_instance_18))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_18 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect17()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT17)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT69)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT70)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_19 = get_idle_instance();

    if(0 > m_instance_19)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_19, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_17)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_19))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration17()
{
    set_hardware_acceleration(m_instance_19, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly17()
{
    if(0 > play(m_instance_19))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause17()
{
    if(0 != pause(m_instance_19))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect17()
{
    if(0 > RTSP_disconnect(m_instance_19))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_19 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect18()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT18)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT71)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT72)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_20 = get_idle_instance();

    if(0 > m_instance_20)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_20, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_18)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_20))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration18()
{
    set_hardware_acceleration(m_instance_20, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly18()
{
    if(0 > play(m_instance_20))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause18()
{
    if(0 != pause(m_instance_20))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect18()
{
    if(0 > RTSP_disconnect(m_instance_20))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_20 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect19()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT19)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT73)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT74)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_21 = get_idle_instance();

    if(0 > m_instance_21)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_21, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_19)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_21))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration19()
{
    set_hardware_acceleration(m_instance_21, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly19()
{
    if(0 > play(m_instance_21))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause19()
{
    if(0 != pause(m_instance_21))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect19()
{
    if(0 > RTSP_disconnect(m_instance_21))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_21 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect20()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT20)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT75)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT76)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_22 = get_idle_instance();

    if(0 > m_instance_22)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_22, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_20)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_22))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration20()
{
    set_hardware_acceleration(m_instance_22, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly20()
{
    if(0 > play(m_instance_22))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause20()
{
    if(0 != pause(m_instance_22))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect20()
{
    if(0 > RTSP_disconnect(m_instance_22))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_22 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect21()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT21)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT77)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT78)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_23 = get_idle_instance();

    if(0 > m_instance_23)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_23, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_21)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_23))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration21()
{
    set_hardware_acceleration(m_instance_23, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly21()
{
    if(0 > play(m_instance_23))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause21()
{
    if(0 != pause(m_instance_23))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect21()
{
    if(0 > RTSP_disconnect(m_instance_23))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_23 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect31()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT31)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT79)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT80)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_24 = get_idle_instance();

    if(0 > m_instance_24)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_24, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_31)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_24))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration31()
{
    set_hardware_acceleration(m_instance_24, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly31()
{
    if(0 > play(m_instance_24))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause31()
{
    if(0 != pause(m_instance_24))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect31()
{
    if(0 > RTSP_disconnect(m_instance_24))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_24 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect22()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT22)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT81)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT82)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_25 = get_idle_instance();

    if(0 > m_instance_25)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_25, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_22)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_25))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration22()
{
    set_hardware_acceleration(m_instance_25, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly22()
{
    if(0 > play(m_instance_25))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause22()
{
    if(0 != pause(m_instance_25))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect22()
{
    if(0 > RTSP_disconnect(m_instance_25))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_25 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect23()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT23)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT83)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT84)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_26 = get_idle_instance();

    if(0 > m_instance_26)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_26, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_23)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_26))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration23()
{
    set_hardware_acceleration(m_instance_26, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly23()
{
    if(0 > play(m_instance_26))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause23()
{
    if(0 != pause(m_instance_26))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect23()
{
    if(0 > RTSP_disconnect(m_instance_26))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_26 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect24()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT24)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT85)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT86)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_27 = get_idle_instance();

    if(0 > m_instance_27)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_27, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_24)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_27))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration24()
{
    set_hardware_acceleration(m_instance_27, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly24()
{
    if(0 > play(m_instance_27))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause24()
{
    if(0 != pause(m_instance_27))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect24()
{
    if(0 > RTSP_disconnect(m_instance_27))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_27 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect25()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT25)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT87)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT88)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_28 = get_idle_instance();

    if(0 > m_instance_28)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_28, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_25)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_28))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration25()
{
    set_hardware_acceleration(m_instance_28, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly25()
{
    if(0 > play(m_instance_28))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause25()
{
    if(0 != pause(m_instance_28))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect25()
{
    if(0 > RTSP_disconnect(m_instance_28))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_28 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect26()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT26)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT89)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT90)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_29 = get_idle_instance();

    if(0 > m_instance_29)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_29, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_26)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_29))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration26()
{
    set_hardware_acceleration(m_instance_29, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly26()
{
    if(0 > play(m_instance_29))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause26()
{
    if(0 != pause(m_instance_29))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect26()
{
    if(0 > RTSP_disconnect(m_instance_29))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_29 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect27()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT27)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT91)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT92)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_30 = get_idle_instance();

    if(0 > m_instance_30)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_30, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_27)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_30))
    {
        AfxMessageBox(L"Connect error");
    }
}

void CtestPlayDlg::OnBnClickedButtonHwacceleration27()
{
    set_hardware_acceleration(m_instance_30, true);
}

void CtestPlayDlg::OnBnClickedButtonPaly27()
{
    if(0 > play(m_instance_30))
    {
        AfxMessageBox(L"Play error");
    }
}

void CtestPlayDlg::OnBnClickedButtonPause27()
{
    if(0 != pause(m_instance_30))
    {
        AfxMessageBox(L"Pause error");
    }
}

void CtestPlayDlg::OnBnClickedButtonDisconnect27()
{
    if(0 > RTSP_disconnect(m_instance_30))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_30 = -1;
}

void CtestPlayDlg::OnBnClickedButtonConnect28()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT28)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT93)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT94)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_31 = get_idle_instance();

    if(0 > m_instance_31)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_31, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_28)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_31))
    {
        AfxMessageBox(L"Connect error");
    }
}

void CtestPlayDlg::OnBnClickedButtonHwacceleration28()
{
    set_hardware_acceleration(m_instance_31, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly28()
{
    if(0 > play(m_instance_31))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause28()
{
    if(0 != pause(m_instance_31))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect28()
{
    if(0 > RTSP_disconnect(m_instance_31))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_31 = -1;
}


void CtestPlayDlg::OnBnClickedButtonConnect32()
{
    CString CStringTemp;
    char strURI[256] = {0};
    char user[50] = {0};
    char pwd[50] = {0};

    GetDlgItem(IDC_EDIT32)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), strURI, 256, NULL, NULL);

    GetDlgItem(IDC_EDIT95)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), user, 50, NULL, NULL);

    GetDlgItem(IDC_EDIT96)->GetWindowTextW(CStringTemp);
    WideCharToMultiByte(CP_UTF8, 0, CStringTemp, CStringTemp.GetLength(), pwd, 50, NULL, NULL);

    m_instance_32 = get_idle_instance();

    if(0 > m_instance_32)
    {
        AfxMessageBox(L"GetRtspINSTANCE error");
        return;
    }

    if(0 > initial_RTSP_parameter(m_instance_32, strURI, user, pwd, GetDlgItem(IDC_PICTURE_AREA_32)->m_hWnd))
    {
        AfxMessageBox(L"InitRtspVideoParam error");
        return;
    }

    if(0 > RTSP_connect(m_instance_32))
    {
        AfxMessageBox(L"Connect error");
    }
}


void CtestPlayDlg::OnBnClickedButtonHwacceleration32()
{
    set_hardware_acceleration(m_instance_32, true);
}


void CtestPlayDlg::OnBnClickedButtonPaly32()
{
    if(0 > play(m_instance_32))
    {
        AfxMessageBox(L"Play error");
    }
}


void CtestPlayDlg::OnBnClickedButtonPause32()
{
    if(0 != pause(m_instance_32))
    {
        AfxMessageBox(L"Pause error");
    }
}


void CtestPlayDlg::OnBnClickedButtonDisconnect32()
{
    if(0 > RTSP_disconnect(m_instance_32))
    {
        AfxMessageBox(L"DisConnect error");
        return;
    }
    m_instance_32 = -1;
}
