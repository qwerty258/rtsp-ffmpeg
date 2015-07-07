
// testPlayDlg.h : header file
//

#pragma once


// CtestPlayDlg dialog
class CtestPlayDlg : public CDialogEx
{
    // Construction
public:
    CtestPlayDlg(CWnd* pParent = NULL);	// standard constructor

    // Dialog Data
    enum
    {
        IDD = IDD_TESTPLAY_DIALOG
    };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


    // Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
private:
    BOOL m_rect_change;
    int m_instance_1;
    int m_instance_2;
    int m_instance_3;
    int m_instance_4;
    int m_instance_5;
    int m_instance_6;
    int m_instance_7;
    int m_instance_8;
    int m_instance_9;
    int m_instance_10;
    int m_instance_11;
    int m_instance_12;
    int m_instance_13;
    int m_instance_14;
    int m_instance_15;
    int m_instance_16;
    int m_instance_17;
    int m_instance_18;
    int m_instance_19;
    int m_instance_20;
    int m_instance_21;
    int m_instance_22;
    int m_instance_23;
    int m_instance_24;
    int m_instance_25;
    int m_instance_26;
    int m_instance_27;
    int m_instance_28;
    int m_instance_29;
    int m_instance_30;
    int m_instance_31;
    int m_instance_32;
public:
    afx_msg void OnClickedButtonPlay();
    virtual BOOL DestroyWindow();
    afx_msg void OnClickedButtonPause();
    afx_msg void OnClickedButtonConnect();
    afx_msg void OnClickedButtonDisconnect();
    afx_msg void OnClickedButtonConnect2();
    afx_msg void OnClickedButtonPaly2();
    afx_msg void OnClickedButtonPause2();
    afx_msg void OnClickedButtonDisconnect2();
    afx_msg void OnClickedButtonResize();
    afx_msg void OnClickedButtonDesktop();
    afx_msg void OnClickedButtonHwacceleration();
    afx_msg void OnBnClickedButtonHwacceleration2();
    afx_msg void OnBnClickedButtonConnect3();
    afx_msg void OnBnClickedButtonHwacceleration3();
    afx_msg void OnBnClickedButtonPaly3();
    afx_msg void OnBnClickedButtonPause3();
    afx_msg void OnBnClickedButtonDisconnect3();
    afx_msg void OnBnClickedButtonConnect4();
    afx_msg void OnBnClickedButtonHwacceleration4();
    afx_msg void OnBnClickedButtonPaly4();
    afx_msg void OnBnClickedButtonPause4();
    afx_msg void OnBnClickedButtonDisconnect4();
    afx_msg void OnBnClickedButtonConnect5();
    afx_msg void OnBnClickedButtonHwacceleration5();
    afx_msg void OnBnClickedButtonPaly5();
    afx_msg void OnBnClickedButtonPause5();
    afx_msg void OnBnClickedButtonDisconnect5();
    afx_msg void OnBnClickedButtonConnect6();
    afx_msg void OnBnClickedButtonHwacceleration6();
    afx_msg void OnBnClickedButtonPaly6();
    afx_msg void OnBnClickedButtonPause6();
    afx_msg void OnBnClickedButtonDisconnect6();
    afx_msg void OnBnClickedButtonConnect7();
    afx_msg void OnBnClickedButtonHwacceleration7();
    afx_msg void OnBnClickedButtonPaly7();
    afx_msg void OnBnClickedButtonPause7();
    afx_msg void OnBnClickedButtonDisconnect7();
    afx_msg void OnBnClickedButtonConnect29();
    afx_msg void OnBnClickedButtonHwacceleration29();
    afx_msg void OnBnClickedButtonPaly29();
    afx_msg void OnBnClickedButtonPause29();
    afx_msg void OnBnClickedButtonDisconnect29();
    afx_msg void OnBnClickedButtonConnect8();
    afx_msg void OnBnClickedButtonHwacceleration8();
    afx_msg void OnBnClickedButtonPaly8();
    afx_msg void OnBnClickedButtonPause8();
    afx_msg void OnBnClickedButtonDisconnect8();
    afx_msg void OnBnClickedButtonConnect9();
    afx_msg void OnBnClickedButtonHwacceleration9();
    afx_msg void OnBnClickedButtonPaly9();
    afx_msg void OnBnClickedButtonPause9();
    afx_msg void OnBnClickedButtonDisconnect9();
    afx_msg void OnBnClickedButtonConnect10();
    afx_msg void OnBnClickedButtonHwacceleration10();
    afx_msg void OnBnClickedButtonPaly10();
    afx_msg void OnBnClickedButtonPause10();
    afx_msg void OnBnClickedButtonDisconnect10();
    afx_msg void OnBnClickedButtonConnect11();
    afx_msg void OnBnClickedButtonHwacceleration11();
    afx_msg void OnBnClickedButtonPaly11();
    afx_msg void OnBnClickedButtonPause11();
    afx_msg void OnBnClickedButtonDisconnect11();
    afx_msg void OnBnClickedButtonConnect12();
    afx_msg void OnBnClickedButtonHwacceleration12();
    afx_msg void OnBnClickedButtonPaly12();
    afx_msg void OnBnClickedButtonPause12();
    afx_msg void OnBnClickedButtonDisconnect12();
    afx_msg void OnBnClickedButtonConnect13();
    afx_msg void OnBnClickedButtonHwacceleration13();
    afx_msg void OnBnClickedButtonPaly13();
    afx_msg void OnBnClickedButtonPause13();
    afx_msg void OnBnClickedButtonDisconnect13();
    afx_msg void OnBnClickedButtonConnect14();
    afx_msg void OnBnClickedButtonHwacceleration14();
    afx_msg void OnBnClickedButtonPaly14();
    afx_msg void OnBnClickedButtonPause14();
    afx_msg void OnBnClickedButtonDisconnect14();
    afx_msg void OnBnClickedButtonConnect30();
    afx_msg void OnBnClickedButtonHwacceleration30();
    afx_msg void OnBnClickedButtonPaly30();
    afx_msg void OnBnClickedButtonPause30();
    afx_msg void OnBnClickedButtonDisconnect30();
    afx_msg void OnBnClickedButtonConnect15();
    afx_msg void OnBnClickedButtonHwacceleration15();
    afx_msg void OnBnClickedButtonPaly15();
    afx_msg void OnBnClickedButtonPause15();
    afx_msg void OnBnClickedButtonDisconnect15();
    afx_msg void OnBnClickedButtonConnect16();
    afx_msg void OnBnClickedButtonHwacceleration16();
    afx_msg void OnBnClickedButtonPaly16();
    afx_msg void OnBnClickedButtonPause16();
    afx_msg void OnBnClickedButtonDisconnect16();
    afx_msg void OnBnClickedButtonConnect17();
    afx_msg void OnBnClickedButtonHwacceleration17();
    afx_msg void OnBnClickedButtonPaly17();
    afx_msg void OnBnClickedButtonPause17();
    afx_msg void OnBnClickedButtonDisconnect17();
    afx_msg void OnBnClickedButtonConnect18();
    afx_msg void OnBnClickedButtonHwacceleration18();
    afx_msg void OnBnClickedButtonPaly18();
    afx_msg void OnBnClickedButtonPause18();
    afx_msg void OnBnClickedButtonDisconnect18();
    afx_msg void OnBnClickedButtonConnect19();
    afx_msg void OnBnClickedButtonHwacceleration19();
    afx_msg void OnBnClickedButtonPaly19();
    afx_msg void OnBnClickedButtonPause19();
    afx_msg void OnBnClickedButtonDisconnect19();
    afx_msg void OnBnClickedButtonConnect20();
    afx_msg void OnBnClickedButtonHwacceleration20();
    afx_msg void OnBnClickedButtonPaly20();
    afx_msg void OnBnClickedButtonPause20();
    afx_msg void OnBnClickedButtonDisconnect20();
    afx_msg void OnBnClickedButtonConnect21();
    afx_msg void OnBnClickedButtonHwacceleration21();
    afx_msg void OnBnClickedButtonPaly21();
    afx_msg void OnBnClickedButtonPause21();
    afx_msg void OnBnClickedButtonDisconnect21();
    afx_msg void OnBnClickedButtonConnect31();
    afx_msg void OnBnClickedButtonHwacceleration31();
    afx_msg void OnBnClickedButtonPaly31();
    afx_msg void OnBnClickedButtonPause31();
    afx_msg void OnBnClickedButtonDisconnect31();
    afx_msg void OnBnClickedButtonConnect22();
    afx_msg void OnBnClickedButtonHwacceleration22();
    afx_msg void OnBnClickedButtonPaly22();
    afx_msg void OnBnClickedButtonPause22();
    afx_msg void OnBnClickedButtonDisconnect22();
    afx_msg void OnBnClickedButtonConnect23();
    afx_msg void OnBnClickedButtonHwacceleration23();
    afx_msg void OnBnClickedButtonPaly23();
    afx_msg void OnBnClickedButtonPause23();
    afx_msg void OnBnClickedButtonDisconnect23();
    afx_msg void OnBnClickedButtonConnect24();
    afx_msg void OnBnClickedButtonHwacceleration24();
    afx_msg void OnBnClickedButtonPaly24();
    afx_msg void OnBnClickedButtonPause24();
    afx_msg void OnBnClickedButtonDisconnect24();
    afx_msg void OnBnClickedButtonConnect25();
    afx_msg void OnBnClickedButtonHwacceleration25();
    afx_msg void OnBnClickedButtonPaly25();
    afx_msg void OnBnClickedButtonPause25();
    afx_msg void OnBnClickedButtonDisconnect25();
    afx_msg void OnBnClickedButtonConnect26();
    afx_msg void OnBnClickedButtonHwacceleration26();
    afx_msg void OnBnClickedButtonPaly26();
    afx_msg void OnBnClickedButtonPause26();
    afx_msg void OnBnClickedButtonDisconnect26();
    afx_msg void OnBnClickedButtonConnect27();
    afx_msg void OnBnClickedButtonConnect28();
    afx_msg void OnBnClickedButtonHwacceleration27();
    afx_msg void OnBnClickedButtonPaly27();
    afx_msg void OnBnClickedButtonPause27();
    afx_msg void OnBnClickedButtonDisconnect27();
    afx_msg void OnBnClickedButtonHwacceleration28();
    afx_msg void OnBnClickedButtonPaly28();
    afx_msg void OnBnClickedButtonPause28();
    afx_msg void OnBnClickedButtonDisconnect28();
    afx_msg void OnBnClickedButtonConnect32();
    afx_msg void OnBnClickedButtonHwacceleration32();
    afx_msg void OnBnClickedButtonPaly32();
    afx_msg void OnBnClickedButtonPause32();
    afx_msg void OnBnClickedButtonDisconnect32();
};

int H264_callback(int instance, char* frame_buff, int frame_buffer_size, int frame_width, int frame_height, void* userdata, int frame_lost);
int YUV420_callback(int instance, char* frame_buffer, int frame_buffer_size, int frame_width, int frame_height, size_t frame_ID, void* userdata, int frame_lost);
int YV12_callback(int instance, char* frame_buff, int frame_buffer_size, int frame_width, int frame_height, void* userdata, int frame_lost);