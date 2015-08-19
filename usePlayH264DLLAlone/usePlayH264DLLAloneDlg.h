
// usePlayH264DLLAloneDlg.h : header file
//

#pragma once


// CusePlayH264DLLAloneDlg dialog
class CusePlayH264DLLAloneDlg : public CDialogEx
{
    // Construction
public:
    CusePlayH264DLLAloneDlg(CWnd* pParent = NULL);	// standard constructor

    // Dialog Data
    enum
    {
        IDD = IDD_USEPLAYH264DLLALONE_DIALOG
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
public:
    UINT* m_pSizeList;
    UCHAR* m_pBuffer;
    int m_instance;
    int m_test_video_data_file_size;
    int m_test_video_size_file_size;
    virtual BOOL DestroyWindow();
};
