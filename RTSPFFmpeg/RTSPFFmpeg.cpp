#include "PlayClient.h"
#include <PlayH264DLL.h>
#include "RTSPFFmpeg.h"
#pragma comment(lib,"PlayH264DLL.lib")

typedef struct container_client_data
{
    CRTSPClient* p_CRTSPClient;
    int idle;
}client_data;

client_data* client_list;
int          max_client_number;

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

    if(0 > initial_decode_DLL(max_client_number))
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

    free_decode_DLL();

    delete[] client_list;

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

                client_list[i].p_CRTSPClient->m_decode_instance = get_idle_decode_instance();
                if(0 > client_list[i].p_CRTSPClient->m_decode_instance)
                {
                    return -1;
                }

                return i;
            }
        }
    }
    return -1;
}

RTSPFFMPEG_API int initial_RTSP_parameter(int instance, char* URI, char* userName, char* password, HWND hWnd)
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

    return client_list[instance].p_CRTSPClient->input_URI(URI, userName, password, hWnd);
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
            GetExitCodeThread(client_list[instance].p_CRTSPClient->m_hThread, &exit_code);
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

RTSPFFMPEG_API int play(int INSTANCE)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == client_list[INSTANCE].p_CRTSPClient)
    {
        return -1;
    }

    return client_list[INSTANCE].p_CRTSPClient->play();
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

    return set_decode_hardware_acceleration(client_list[instance].p_CRTSPClient->m_decode_instance, acceleration);
}

RTSPFFMPEG_API int playing_windows_RECT_changed(int instance)
{
    if(0 > checkINSTANCE(instance) || NULL == client_list[instance].p_CRTSPClient)
    {
        return -1;
    }

    return playing_windows_RECT_changed_of_decode_DLL(client_list[instance].p_CRTSPClient->m_decode_instance);
}

RTSPFFMPEG_API int set_YUV420_callback(int instance, function_YUV420 p_function_YUV420, void* additional_data, bool trace_lost_package)
{
#ifdef _DEBUG
    if(0 > checkINSTANCE(instance))
#else
    if(0 > checkINSTANCE(instance) || NULL == p_function_YUV420)
#endif
    {
        return -1;
    }

    set_decode_YUV420_callback(client_list[instance].p_CRTSPClient->m_decode_instance, p_function_YUV420, additional_data, trace_lost_package);

    return 0;
}

RTSPFFMPEG_API int set_YV12_callback(int instance, function_YV12 p_function_YV12, void* additional_data, bool trace_lost_package)
{
    if(0 > checkINSTANCE(instance) || NULL == p_function_YV12)
    {
        return -1;
    }

    set_decode_YV12_callback(client_list[instance].p_CRTSPClient->m_decode_instance, p_function_YV12, additional_data, trace_lost_package);

    return 0;
}

RTSPFFMPEG_API int set_H264_callback(int instance, function_H264 p_function_H264, void* additional_data, bool trace_lost_package)
{
    if(0 > checkINSTANCE(instance) || NULL == p_function_H264)
    {
        return -1;
    }

    set_decode_H264_callback(client_list[instance].p_CRTSPClient->m_decode_instance, p_function_H264, additional_data, trace_lost_package);

    return 0;
}