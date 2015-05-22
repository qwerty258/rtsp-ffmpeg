#pragma once

#include "RtspRequest.h"
#include "Rtp.h"

#include "VBuffer.h"

#include "Decode.h"

class CRTSPCLient
{
public:
    CRTSPCLient();
    ~CRTSPCLient();
    int InputURL(char* URL, char* UserName, char* PWD);
    int PlayURL(HWND hWnd);
    int stopURL();

public:
    HWND m_hWnd;
    int m_INSTANCE;
    int m_ans; // 0: initial state    1: in the cycle    2: exit thread
    bool m_circulation;
    DWORD m_threadID;

    char* m_URI;
    char* m_userName;
    char* m_password;

    PFCALLBACK func;
    TDrawLineCallBack funcD;
    TBmpCallBack bmpFunc;
    TDrawRectCallBack fillbmp;
    TYUVCallBack YUVFunc;
    TH264CallBack H264Func;
    void * YUVEx;
    bool nHWAcceleration;
};

DWORD WINAPI RTSPVideo(LPVOID lpParam);