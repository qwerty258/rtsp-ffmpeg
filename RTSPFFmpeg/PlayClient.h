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
    bool nHWAcceleration;

    // funcion pointer for callback begin
    function_YUV420 m_p_function_YUV420;
    void* m_p_YUV420_extra_data;
    bool m_trace_lost_package;
    PFCALLBACK func;
    TDrawLineCallBack funcD;
    TBmpCallBack bmpFunc;
    TDrawRectCallBack fillbmp;
    TYUVCallBack YUVFunc;
    TH264CallBack H264Func;
    // funcion pointer for callback end


    // function pointer for PlayH264DLL begin
    function_get_idle_instance        m_p_function_get_idle_instance;
    function_initial_decode_parameter m_p_function_initial_decode_parameter;
    function_decode                   m_p_function_decode;
    function_free_decode_instance     m_p_function_free_decode_instance;
    function_set_YUV420_callback      m_p_function_set_YUV420_callback;

    setH264CallBack       m_p_func_setH264CallBack;
    fSetCallBack          m_p_func_SetCallBack;
    fpauseVideos          m_p_func_pauseVideos;
    fplayVideos           m_p_func_playVideos;
    finputBuf             m_p_func_inputBuf;
    fresize               m_p_func_resize;
    fSetDrawLineCallBack  m_p_func_SetDrawLineCallBack;
    fSetBmpCallBack       m_p_func_SetBmpCallBack;
    fSetFillBmpCallBack   m_p_func_SetFillBmpCallBack;
    revoHW                m_p_func_revoHWFunc;
    // function pointer for PlayH264DLL end
};

DWORD WINAPI RTSPVideo(LPVOID lpParam);