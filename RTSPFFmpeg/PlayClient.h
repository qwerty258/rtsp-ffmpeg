#pragma once
#include <ctime>
#include "RtspRequest.h"
#include "Rtp.h"

#include "VBuffer.h"

#include "Decode.h"
#include "UNpack.h"
#include <PlayH264DLL.h>
#pragma comment(lib,"PlayH264DLL.lib")

class CRTSPClient
{
private:
    bool m_bInitURI;
    bool m_bConnected;
    bool m_bPlaying;
public:
    bool m_bPause;

public:
    CRTSPClient();
    ~CRTSPClient();
    int input_URI(char* URI, char* username, char* password, HWND hWnd);
    int connect(void);
    int play(void);
    int pause(void);
    int disconnect(void);

    // windows resource begin
    HMODULE m_hDLL;
    HANDLE m_hThread;
    DWORD m_threadID;
    HWND m_hWnd;
    // windows resource end

    int m_decode_instance;
    int m_ans; // 0: initial state    1: in the cycle    2: exit thread

    // state control
    bool m_circulation;

    CRTSPRequest* m_RTSPRequest;

    CRTPPackage* m_p_CRTPPackage;

    char* m_URI;
    char* m_userName;
    char* m_password;
    myparamInput* m_myparamInput;
};

DWORD WINAPI RTSPVideo(LPVOID lpParam);