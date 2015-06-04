#include "RTSPFFmpeg.h"

typedef struct container_client_data
{
    CRTSPCLient* pt;
    int idle;
}client_data;

client_data* client_list;
int          max_client_number;
HMODULE      hPlayH264DLL;

// function pointers for PlayH264DLL
function_initial_decode_DLL       p_function_initial_decode_DLL;
function_free_decode_DLL          p_function_free_decode_DLL;
function_get_idle_instance        p_function_get_idle_instance;
function_initial_decode_parameter p_function_initial_decode_parameter;
function_decode                   p_function_decode;
function_free_decode_instance     p_function_free_decode_instance;
function_set_YUV420_callback      p_function_set_YUV420_callback;
setH264CallBack       p_func_setH264CallBack;
fSetCallBack          p_func_SetCallBack;
fpauseVideos          p_func_pauseVideos;
fplayVideos           p_func_playVideos;
finputBuf             p_func_inputBuf;
fresize               p_func_resize;
fSetDrawLineCallBack  p_func_SetDrawLineCallBack;
fSetBmpCallBack       p_func_SetBmpCallBack;
fSetFillBmpCallBack   p_func_SetFillBmpCallBack;
revoHW                p_func_revoHWFunc;

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

    p_func_SetCallBack = (fSetCallBack)GetProcAddress(hPlayH264DLL, "SetCallBack");
    if(NULL == p_func_SetCallBack)
    {
        MessageBox(0, L"GetProcAddress SetCallBack error", NULL, MB_OK);
        exit(-1);
    }

    p_func_SetDrawLineCallBack = (fSetDrawLineCallBack)GetProcAddress(hPlayH264DLL, "SetDrawLineCallBack");
    if(NULL == p_func_SetDrawLineCallBack)
    {
        MessageBox(0, L"GetProcAddress SetDrawLineCallBack error", NULL, MB_OK);
        exit(-1);
    }

    p_func_SetBmpCallBack = (fSetBmpCallBack)GetProcAddress(hPlayH264DLL, "SetBmpCallBack");
    if(NULL == p_func_SetBmpCallBack)
    {
        MessageBox(0, L"GetProcAddress SetBmpCallBack error", NULL, MB_OK);
        exit(-1);
    }

    p_func_SetFillBmpCallBack = (fSetFillBmpCallBack)GetProcAddress(hPlayH264DLL, "SetFillBmpCallBack");
    if(NULL == p_func_SetFillBmpCallBack)
    {
        MessageBox(0, L"GetProcAddress SetFillBmpCallBack error", NULL, MB_OK);
        exit(-1);
    }


    p_func_setH264CallBack = (setH264CallBack)GetProcAddress(hPlayH264DLL, "SetH264CallBack");
    if(NULL == p_func_setH264CallBack)
    {
        MessageBox(0, L"GetProcAddress SetH264CallBack error", NULL, MB_OK);
        exit(-1);
    }

    p_func_revoHWFunc = (revoHW)GetProcAddress(hPlayH264DLL, "RevoHWAcceleration");
    if(NULL == p_func_revoHWFunc)
    {
        MessageBox(0, L"GetProcAddress RevoHWAcceleration error", NULL, MB_OK);
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
        if(0 != client_list[i].idle)
        {
            client_list[i].idle = 0;
        }

        if(NULL != client_list[i].pt)
        {
            client_list[i].pt->disconnect();
            delete client_list[i].pt;
            client_list[i].pt = NULL;
        }
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
                client_list[i].pt = new CRTSPCLient;
                if(NULL == client_list[i].pt)
                {
                    MessageBox(NULL, L"memory new error", NULL, MB_OK);
                    return -1;
                }
                client_list[i].pt->m_p_function_get_idle_instance = p_function_get_idle_instance;
                client_list[i].pt->m_p_function_initial_decode_parameter = p_function_initial_decode_parameter;
                client_list[i].pt->m_p_function_decode = p_function_decode;
                client_list[i].pt->m_p_function_free_decode_instance = p_function_free_decode_instance;
                client_list[i].pt->m_p_function_set_YUV420_callback = p_function_set_YUV420_callback;


                client_list[i].pt->m_p_func_inputBuf = p_func_inputBuf;
                client_list[i].pt->m_p_func_pauseVideos = p_func_pauseVideos;
                client_list[i].pt->m_p_func_playVideos = p_func_playVideos;
                client_list[i].pt->m_p_func_resize = p_func_resize;
                client_list[i].pt->m_p_func_revoHWFunc = p_func_revoHWFunc;
                client_list[i].pt->m_p_func_SetBmpCallBack = p_func_SetBmpCallBack;
                client_list[i].pt->m_p_func_SetCallBack = p_func_SetCallBack;
                client_list[i].pt->m_p_func_SetDrawLineCallBack = p_func_SetDrawLineCallBack;
                client_list[i].pt->m_p_func_SetFillBmpCallBack = p_func_SetFillBmpCallBack;
                client_list[i].pt->m_p_func_setH264CallBack = p_func_setH264CallBack;
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

    if(NULL == client_list[instance].pt)
    {
        MessageBox(NULL, L"InitRtspVideoParam: new error!", NULL, MB_OK);
        return -1;
    }

    return client_list[instance].pt->input_URI(URI, userName, password);
}

RTSPFFMPEG_API int RTSP_connect(int instance)
{
    if(0 > checkINSTANCE(instance) || NULL == client_list[instance].pt)
    {
        return -1;
    }

    return client_list[instance].pt->connect();
}

RTSPFFMPEG_API int play(int INSTANCE, HWND hWnd)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == client_list[INSTANCE].pt)
    {
        return -1;
    }

    return client_list[INSTANCE].pt->play(hWnd);
}

RTSPFFMPEG_API int pause(int INSTANCE)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == client_list[INSTANCE].pt)
    {
        return -1;
    }

    return client_list[INSTANCE].pt->pause();
}

RTSPFFMPEG_API int RTSP_disconnect(int INSTANCE)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == client_list[INSTANCE].pt)
    {
        return -1;
    }

    client_list[INSTANCE].pt->disconnect();

    client_list[INSTANCE].idle = 0;

    delete client_list[INSTANCE].pt;
    client_list[INSTANCE].pt = NULL;

    return 0;
}

RTSPFFMPEG_API int set_YUV420_callback(int instance, function_YUV420 p_function_YUV420, void* additional_data, bool trace_lost_package)
{
    if(0 > checkINSTANCE(instance) || NULL == p_function_YUV420)
    {
        return -1;
    }

    client_list[instance].pt->m_p_function_YUV420 = p_function_YUV420;
    client_list[instance].pt->m_p_YUV420_extra_data = additional_data;
    client_list[instance].pt->m_trace_lost_package = trace_lost_package;

    return 0;
}

// set hardware acceleration
// acceleration: true: use hardware acceleration, false: use software decode
RTSPFFMPEG_API int RevoHWAcceleration(int INSTANCE, bool acceleration)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == client_list[INSTANCE].pt)
    {
        return -1;
    }

    client_list[INSTANCE].pt->nHWAcceleration = acceleration;

    return 0;
}


RTSPFFMPEG_API int pSetDrawLineCallBack(int INSTANCE, TDrawLineCallBack f1)
{
    if(NULL == f1)
    {
        return -1;
    }

    if(checkINSTANCE(INSTANCE) < 0)
    {
        return -1;
    }

    client_list[INSTANCE].pt->funcD = f1;

    return 0;
}

RTSPFFMPEG_API int pSetPFCALLBACK(int INSTANCE, PFCALLBACK f1)
{
    if(NULL == f1)
    {
        return -1;
    }

    if(checkINSTANCE(INSTANCE) < 0)
    {
        return -1;
    }

    client_list[INSTANCE].pt->func = f1;

    return 0;
}

RTSPFFMPEG_API int pSetBmpCallBack(int INSTANCE, TBmpCallBack f2)
{
    if(NULL == f2)
    {
        return -1;
    }

    if(checkINSTANCE(INSTANCE) < 0)
    {
        return -1;
    }

    client_list[INSTANCE].pt->bmpFunc = f2;

    return 0;
}

RTSPFFMPEG_API int pSetFillBmpCallBack(int INSTANCE, TDrawRectCallBack f3)
{
    if(NULL == f3)
    {
        return -1;
    }

    if(checkINSTANCE(INSTANCE) < 0)
    {
        return -1;
    }

    client_list[INSTANCE].pt->fillbmp = f3;

    return 0;
}

//获取H264数据
RTSPFFMPEG_API int pSetH264CallBack(int INSTANCE, TH264CallBack f3)
{
    if(NULL == f3)
    {
        return -1;
    }

    if(checkINSTANCE(INSTANCE) < 0)
    {
        return -1;
    }

    client_list[INSTANCE].pt->H264Func = f3;

    return 0;
}
