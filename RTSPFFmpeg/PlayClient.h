#pragma once
#include <ctime>
#include "RtspRequest.h"
#include "Rtp.h"

#include "VBuffer.h"

#include "Decode.h"

class CRTSPCLient
{
private:
    void load_PlayH264DLL();
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
    YUV420Func YUVFunc;
    TH264CallBack H264Func;
    void * YUVEx;
    bool nHWAcceleration;
};

DWORD WINAPI RTSPVideo(LPVOID lpParam);