#pragma once
#include <ctime>
#include "RtspRequest.h"
#include "Rtp.h"

#include "VBuffer.h"

#include "Decode.h"

class CRTSPCLient
{
private:
    bool m_bInitURI;
    bool m_bConnected;
    bool m_bPlaying;

public:
    CRTSPCLient();
    ~CRTSPCLient();
    int input_URI(char* URI, char* username, char* password);
    int connect();
    int play(HWND hWnd);
    int pause();
    int disconnect();

    HMODULE m_hDLL;
    HWND m_hWnd;
    int m_INSTANCE;
    int m_ans; // 0: initial state    1: in the cycle    2: exit thread

    // state control
    bool m_circulation;

    DWORD m_threadID;
    CRTSPRequest* m_RTSPRequest;

    char* m_URI;
    char* m_userName;
    char* m_password;
    myparamInput* m_myparamInput;

    PFCALLBACK func;
    TDrawLineCallBack funcD;
    TBmpCallBack bmpFunc;
    TDrawRectCallBack fillbmp;
    TYUVCallBack YUVFunc;
    TH264CallBack H264Func;
    void * YUVEx;
    bool nHWAcceleration;

    // function pointer for PlayH264DLL
public:
    setH264CallBack       m_p_func_setH264CallBack;
    setYUVCallBack        m_p_func_setYUVCallBack;
    fSetCallBack          m_p_func_SetCallBack;
    fGetIdlevideoINSTANCE m_p_func_GetIdlevideoINSTANCE;
    fInitVideoParamNew    m_p_func_InitVideoParamNew;
    fInitVideoParam       m_p_func_InitVideoParam;
    fpauseVideos          m_p_func_pauseVideos;
    fplayVideos           m_p_func_playVideos;
    ffreeVideos           m_p_func_freeVideos;
    finputBuf             m_p_func_inputBuf;
    fresize               m_p_func_resize;
    fSetDrawLineCallBack  m_p_func_SetDrawLineCallBack;
    fSetBmpCallBack       m_p_func_SetBmpCallBack;
    fSetFillBmpCallBack   m_p_func_SetFillBmpCallBack;
    revoHW                m_p_func_revoHWFunc;
};

DWORD WINAPI RTSPVideo(LPVOID lpParam);