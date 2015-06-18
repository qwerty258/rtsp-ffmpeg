
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
    int m_instance_1;
    BOOL m_rect_change;
    int m_instance_2;
    SDL_Window* screen;
    SDL_Renderer* sdlRenderer;
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
};

int H264_callback(int instance, char* frame_buff, int frame_buffer_size, int frame_width, int frame_height, void* userdata, int frame_lost);
int YUV420_callback(int instance, char* frame_buffer, int frame_buffer_size, int frame_width, int frame_height, size_t frame_ID, void* userdata, int frame_lost);
int YV12_callback(int instance, char* frame_buff, int frame_buffer_size, int frame_width, int frame_height, void* userdata, int frame_lost);