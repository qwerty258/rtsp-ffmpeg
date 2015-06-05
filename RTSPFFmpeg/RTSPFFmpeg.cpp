#include "RTSPFFmpeg.h"

typedef struct container_client_data
{
    CRTSPClient* p_CRTSPClient;
    int idle;
}client_data;

client_data* client_list;
int          max_client_number;
HMODULE      hPlayH264DLL;

// function pointers for PlayH264DLL begin
function_initial_decode_DLL         p_function_initial_decode_DLL;
function_free_decode_DLL            p_function_free_decode_DLL;
function_get_idle_instance          p_function_get_idle_instance;
function_initial_decode_parameter   p_function_initial_decode_parameter;
function_decode                     p_function_decode;
function_free_decode_instance       p_function_free_decode_instance;
function_set_YUV420_callback        p_function_set_YUV420_callback;
function_set_YV12_callback          p_function_set_YV12_callback;
function_set_H264_callback          p_function_set_H264_callback;
function_set_hardware_acceleration  p_function_set_hardware_acceleration;
function_pauseVideos                p_function_pauseVideos;
function_playVideos                 p_function_playVideos;
function_inputBuf                   p_function_inputBuf;
function_resize                     p_function_resize;
// function pointers for PlayH264DLL end

int checkINSTANCE(int instance)
{
    if(0 > instance || max_client_number < instance || NULL == client_list)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

RTSPFFMPEG_API int initial_RTSP_DLL(int max_number_of_playing_instance)
{
    if(0 >= max_number_of_playing_instance)
    {
        return -1;
    }

    max_client_number = max_number_of_playing_instance;

    client_list = new client_data[max_client_number];
    if(NULL == client_list)
    {
        MessageBox(NULL, L"memory new error", NULL, MB_OK);
    }

    memset(client_list, 0x0, max_client_number * sizeof(client_data));

    hPlayH264DLL = LoadLibrary(L"PlayH264DLL.dll");
    if(NULL == hPlayH264DLL)
    {
        TCHAR* temp = new TCHAR[2048];
        wsprintf(temp, L"LoadLibrary PlayH24DLL.dll error, error code: %d", GetLastError());
        MessageBox(0, temp, NULL, MB_OK);
        delete[] temp;
        exit(-1);
    }

    p_function_initial_decode_DLL = (function_initial_decode_DLL)GetProcAddress(hPlayH264DLL, "initial_decode_DLL");
    if(NULL == p_function_initial_decode_DLL)
    {
        MessageBox(0, L"GetProcAddress initVideoDLL error", NULL, MB_OK);
        exit(-1);
    }

    p_function_free_decode_DLL = (function_free_decode_DLL)GetProcAddress(hPlayH264DLL, "free_decode_DLL");
    if(NULL == p_function_free_decode_DLL)
    {
        MessageBox(NULL, L"GetProcAddress exitdll error", NULL, MB_OK);
    }

    p_function_get_idle_instance = (function_get_idle_instance)GetProcAddress(hPlayH264DLL, "get_idle_instance");
    if(NULL == p_function_get_idle_instance)
    {
        MessageBox(NULL, L"GetProcAddress GetIdlevideoINSTANCE error", NULL, MB_OK);
        exit(-1);
    }

    p_function_initial_decode_parameter = (function_initial_decode_parameter)GetProcAddress(hPlayH264DLL, "initial_decode_parameter");
    if(NULL == p_function_initial_decode_parameter)
    {
        MessageBox(0, L"GetProcAddress InitVideoParam error", NULL, MB_OK);
        exit(-1);
    }

    p_function_decode = (function_decode)GetProcAddress(hPlayH264DLL, "decode");
    if(NULL == p_function_decode)
    {
        MessageBox(0, L"GetProcAddress decode error", NULL, MB_OK);
        exit(-1);
    }

    p_function_free_decode_instance = (function_free_decode_instance)GetProcAddress(hPlayH264DLL, "free_decode_instance");
    if(NULL == p_function_free_decode_instance)
    {
        MessageBox(0, L"GetProcAddress freeVideos error", NULL, MB_OK);
        exit(-1);
    }

    p_function_set_YUV420_callback = (function_set_YUV420_callback)GetProcAddress(hPlayH264DLL, "set_YUV420_callback");
    if(NULL == p_function_set_YUV420_callback)
    {
        MessageBox(0, L"GetProcAddress SetYUVCallBack error", NULL, MB_OK);
        exit(-1);
    }

    p_function_set_YV12_callback = (function_set_YV12_callback)GetProcAddress(hPlayH264DLL, "set_YV12_callback");
    if(NULL == p_function_set_YV12_callback)
    {
        MessageBox(0, L"GetProcAddress SetCallBack error", NULL, MB_OK);
        exit(-1);
    }

    p_function_set_H264_callback = (function_set_H264_callback)GetProcAddress(hPlayH264DLL, "set_H264_callback");
    if(NULL == p_function_set_H264_callback)
    {
        MessageBox(0, L"GetProcAddress SetDrawLineCallBack error", NULL, MB_OK);
        exit(-1);
    }

    p_function_set_hardware_acceleration = (function_set_hardware_acceleration)GetProcAddress(hPlayH264DLL, "set_hardware_acceleration");
    if(NULL == p_function_set_hardware_acceleration)
    {
        MessageBox(0, L"GetProcAddress RevoHWAcceleration error", NULL, MB_OK);
        exit(-1);
    }

    p_function_pauseVideos = (function_pauseVideos)GetProcAddress(hPlayH264DLL, "pauseVideos");
    if(NULL == p_function_pauseVideos)
    {
        MessageBox(0, L"GetProcAddress SetBmpCallBack error", NULL, MB_OK);
        exit(-1);
    }

    p_function_playVideos = (function_playVideos)GetProcAddress(hPlayH264DLL, "playVideos");
    if(NULL == p_function_playVideos)
    {
        MessageBox(0, L"GetProcAddress SetFillBmpCallBack error", NULL, MB_OK);
        exit(-1);
    }


    p_function_inputBuf = (function_inputBuf)GetProcAddress(hPlayH264DLL, "inputBuf");
    if(NULL == p_function_inputBuf)
    {
        MessageBox(0, L"GetProcAddress SetH264CallBack error", NULL, MB_OK);
        exit(-1);
    }

    p_function_resize = (function_resize)GetProcAddress(hPlayH264DLL, "resize");
    if(NULL == p_function_resize)
    {
        MessageBox(0, L"GetProcAddress resize error", NULL, MB_OK);
        exit(-1);
    }


    if(0 > p_function_initial_decode_DLL(max_client_number))
    {
        return -1;
    }

    return 0;
}

RTSPFFMPEG_API int free_RTSP_DLL(void)
{
    for(int i = 0; i < max_client_number; ++i)
    {
        free_RTSP_instance(i);
    }

    p_function_free_decode_DLL();

    delete[] client_list;

    FreeLibrary(hPlayH264DLL);

    return 0;
}

RTSPFFMPEG_API int get_idle_instance(void)
{
    for(int i = 0; i < max_client_number; ++i)
    {
        if(0 == client_list[i].idle)
        {
            if(0 == checkINSTANCE(i))
            {
                client_list[i].idle = 1;
                client_list[i].p_CRTSPClient = new CRTSPClient;
                if(NULL == client_list[i].p_CRTSPClient)
                {
                    MessageBox(NULL, L"memory new error", NULL, MB_OK);
                    return -1;
                }

                client_list[i].p_CRTSPClient->m_p_function_initial_decode_DLL = p_function_initial_decode_DLL;
                client_list[i].p_CRTSPClient->m_p_function_free_decode_DLL = p_function_free_decode_DLL;
                client_list[i].p_CRTSPClient->m_p_function_get_idle_instance = p_function_get_idle_instance;
                client_list[i].p_CRTSPClient->m_p_function_initial_decode_parameter = p_function_initial_decode_parameter;
                client_list[i].p_CRTSPClient->m_p_function_decode = p_function_decode;
                client_list[i].p_CRTSPClient->m_p_function_free_decode_instance = p_function_free_decode_instance;
                client_list[i].p_CRTSPClient->m_p_function_set_YUV420_callback = p_function_set_YUV420_callback;
                client_list[i].p_CRTSPClient->m_p_function_set_YV12_callback = p_function_set_YV12_callback;
                client_list[i].p_CRTSPClient->m_p_function_set_H264_callback = p_function_set_H264_callback;
                client_list[i].p_CRTSPClient->m_p_function_set_hardware_acceleration = p_function_set_hardware_acceleration;
                client_list[i].p_CRTSPClient->m_p_function_pauseVideos = p_function_pauseVideos;
                client_list[i].p_CRTSPClient->m_p_function_playVideos = p_function_playVideos;
                client_list[i].p_CRTSPClient->m_p_function_inputBuf = p_function_inputBuf;
                client_list[i].p_CRTSPClient->m_p_function_resize = p_function_resize;

                return i;
            }
        }
    }
    return -1;
}

RTSPFFMPEG_API int initial_RTSP_parameter(int instance, char* URI, char* userName, char* password)
{
    if(0 > checkINSTANCE(instance))
    {
        return -1;
    }

    if(NULL == client_list[instance].p_CRTSPClient)
    {
        MessageBox(NULL, L"InitRtspVideoParam: new error!", NULL, MB_OK);
        return -1;
    }

    return client_list[instance].p_CRTSPClient->input_URI(URI, userName, password);
}

RTSPFFMPEG_API int free_RTSP_instance(int instance)
{
    if(0 > checkINSTANCE(instance))
    {
        return -1;
    }

    if(0 != client_list[instance].idle)
    {
        client_list[instance].idle = 0;
    }

    if(NULL != client_list[instance].p_CRTSPClient)
    {
        client_list[instance].p_CRTSPClient->disconnect();

        DWORD exit_code = STILL_ACTIVE;
        while(STILL_ACTIVE == exit_code)
        {
            Sleep(200);
            GetExitCodeProcess(client_list[instance].p_CRTSPClient->m_hThread, &exit_code);
        }

        CloseHandle(client_list[instance].p_CRTSPClient->m_hThread);

        delete client_list[instance].p_CRTSPClient;
        client_list[instance].p_CRTSPClient = NULL;
    }

    return 0;
}

RTSPFFMPEG_API int RTSP_connect(int instance)
{
    if(0 > checkINSTANCE(instance) || NULL == client_list[instance].p_CRTSPClient)
    {
        return -1;
    }

    return client_list[instance].p_CRTSPClient->connect();
}

RTSPFFMPEG_API int play(int INSTANCE, HWND hWnd)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == client_list[INSTANCE].p_CRTSPClient)
    {
        return -1;
    }

    return client_list[INSTANCE].p_CRTSPClient->play(hWnd);
}

RTSPFFMPEG_API int pause(int INSTANCE)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == client_list[INSTANCE].p_CRTSPClient)
    {
        return -1;
    }

    return client_list[INSTANCE].p_CRTSPClient->pause();
}

RTSPFFMPEG_API int RTSP_disconnect(int INSTANCE)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == client_list[INSTANCE].p_CRTSPClient)
    {
        return -1;
    }
    else
    {
        return free_RTSP_instance(INSTANCE);
    }
}

RTSPFFMPEG_API int set_hardware_acceleration(int instance, bool acceleration)
{
    if(0 > checkINSTANCE(instance) || NULL == client_list[instance].p_CRTSPClient)
    {
        return -1;
    }

    client_list[instance].p_CRTSPClient->nHWAcceleration = acceleration;

    return 0;
}

RTSPFFMPEG_API int set_YUV420_callback(int instance, function_YUV420 p_function_YUV420, void* additional_data, bool trace_lost_package)
{
    if(0 > checkINSTANCE(instance) || NULL == p_function_YUV420)
    {
        return -1;
    }

    client_list[instance].p_CRTSPClient->m_p_function_YUV420 = p_function_YUV420;
    client_list[instance].p_CRTSPClient->m_p_YUV420_extra_data = additional_data;
    client_list[instance].p_CRTSPClient->m_b_YUV420_trace_lost_package = trace_lost_package;

    return 0;
}

RTSPFFMPEG_API int set_YV12_callback(int instance, function_YV12 p_function_YV12, void* additional_data, bool trace_lost_package)
{
    if(0 > checkINSTANCE(instance) || NULL == p_function_YV12)
    {
        return -1;
    }

    client_list[instance].p_CRTSPClient->m_p_function_YV12 = p_function_YV12;
    client_list[instance].p_CRTSPClient->m_p_YV12_extra_data = additional_data;
    client_list[instance].p_CRTSPClient->m_b_YV12_trace_lost_package = trace_lost_package;

    return 0;
}

RTSPFFMPEG_API int set_H264_callback(int instance, function_H264 p_function_H264, void* additional_data, bool trace_lost_package)
{
    if(0 > checkINSTANCE(instance) || NULL == p_function_H264)
    {
        return -1;
    }

    client_list[instance].p_CRTSPClient->m_p_function_H264 = p_function_H264;
    client_list[instance].p_CRTSPClient->m_p_H264_extra_data = additional_data;
    client_list[instance].p_CRTSPClient->m_b_H264_trace_lost_package = trace_lost_package;

    return 0;
}

//RTSPFFMPEG_API int pSetPFCALLBACK(int INSTANCE, PFCALLBACK f1)
//{
//    if(NULL == f1)
//    {
//        return -1;
//    }
//
//    if(checkINSTANCE(INSTANCE) < 0)
//    {
//        return -1;
//    }
//
//    client_list[INSTANCE].pt->func = f1;
//
//    return 0;
//}
//
//RTSPFFMPEG_API int pSetBmpCallBack(int INSTANCE, TBmpCallBack f2)
//{
//    if(NULL == f2)
//    {
//        return -1;
//    }
//
//    if(checkINSTANCE(INSTANCE) < 0)
//    {
//        return -1;
//    }
//
//    client_list[INSTANCE].pt->bmpFunc = f2;
//
//    return 0;
//}
//
//RTSPFFMPEG_API int pSetFillBmpCallBack(int INSTANCE, TDrawRectCallBack f3)
//{
//    if(NULL == f3)
//    {
//        return -1;
//    }
//
//    if(checkINSTANCE(INSTANCE) < 0)
//    {
//        return -1;
//    }
//
//    client_list[INSTANCE].pt->fillbmp = f3;
//
//    return 0;
//}