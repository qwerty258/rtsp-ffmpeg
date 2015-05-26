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
public:
    CRTSPCLient();
    ~CRTSPCLient();
    int input_URI(char* URI, char* username, char* password);
    int connect();
    int play(HWND hWnd);
    int pause();
    int stop();

    HMODULE m_hDLL;
    HWND m_hWnd;
    int m_INSTANCE;
    int m_ans; // 0: initial state    1: in the cycle    2: exit thread
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
};

DWORD WINAPI RTSPVideo(LPVOID lpParam);