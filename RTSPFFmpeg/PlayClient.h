#pragma once
#include <ctime>
#include "RtspRequest.h"
#include "Rtp.h"

#include "VBuffer.h"

#include "Decode.h"
#include "UNpack.h"

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

    int m_INSTANCE;
    int m_ans; // 0: initial state    1: in the cycle    2: exit thread

    // state control
    bool m_circulation;

    CRTSPRequest* m_RTSPRequest;

    CRTPPackage* m_p_CRTPPackage;

    char* m_URI;
    char* m_userName;
    char* m_password;
    myparamInput* m_myparamInput;
    bool nHWAcceleration;

    // funcion pointer for callback begin
    function_YUV420 m_p_function_YUV420;
    void* m_p_YUV420_extra_data;
    bool m_b_YUV420_trace_lost_package;

    function_YV12 m_p_function_YV12;
    void* m_p_YV12_extra_data;
    bool m_b_YV12_trace_lost_package;

    function_H264 m_p_function_H264;
    void* m_p_H264_extra_data;
    bool m_b_H264_trace_lost_package;
    // funcion pointer for callback end


    // function pointer for PlayH264DLL begin
    function_initial_decode_DLL         m_p_function_initial_decode_DLL;
    function_free_decode_DLL            m_p_function_free_decode_DLL;
    function_get_idle_instance          m_p_function_get_idle_instance;
    function_initial_decode_parameter   m_p_function_initial_decode_parameter;
    function_decode                     m_p_function_decode;
    function_free_decode_instance       m_p_function_free_decode_instance;
    function_set_YUV420_callback        m_p_function_set_YUV420_callback;
    function_set_YV12_callback          m_p_function_set_YV12_callback;
    function_set_H264_callback          m_p_function_set_H264_callback;
    function_set_hardware_acceleration  m_p_function_set_hardware_acceleration;
    function_pauseVideos                m_p_function_pauseVideos;
    function_playVideos                 m_p_function_playVideos;
    function_inputBuf                   m_p_function_inputBuf;
    function_resize                     m_p_function_resize;
    // function pointer for PlayH264DLL end
};

DWORD WINAPI RTSPVideo(LPVOID lpParam);