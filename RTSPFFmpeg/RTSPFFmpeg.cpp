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
setH264CallBack       p_func_setH264CallBack;
setYUVCallBack        p_func_setYUVCallBack;
fSetCallBack          p_func_SetCallBack;
finitVideoDLL         p_func_initVideoDLL;
fGetIdlevideoINSTANCE p_func_GetIdlevideoINSTANCE;
fInitVideoParamNew    p_func_InitVideoParamNew;
fInitVideoParam       p_func_InitVideoParam;
fpauseVideos          p_func_pauseVideos;
fplayVideos           p_func_playVideos;
ffreeVideos           p_func_freeVideos;
finputBuf             p_func_inputBuf;
fresize               p_func_resize;
fexitdll              p_func_exitdll;
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

RTSPFFMPEG_API int InitRtspDLL(int max_number_of_playing_instance)
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

    p_func_GetIdlevideoINSTANCE = (fGetIdlevideoINSTANCE)GetProcAddress(hPlayH264DLL, "GetIdlevideoINSTANCE");
    if(NULL == p_func_GetIdlevideoINSTANCE)
    {
        MessageBox(NULL, L"GetProcAddress GetIdlevideoINSTANCE error", NULL, MB_OK);
        exit(-1);
    }

    p_func_initVideoDLL = (finitVideoDLL)GetProcAddress(hPlayH264DLL, "initVideoDLL");
    if(NULL == p_func_initVideoDLL)
    {
        MessageBox(0, L"GetProcAddress initVideoDLL error", NULL, MB_OK);
        exit(-1);
    }

    p_func_InitVideoParam = (fInitVideoParam)GetProcAddress(hPlayH264DLL, "InitVideoParam");
    if(NULL == p_func_InitVideoParam)
    {
        MessageBox(0, L"GetProcAddress InitVideoParam error", NULL, MB_OK);
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

    p_func_setYUVCallBack = (setYUVCallBack)GetProcAddress(hPlayH264DLL, "SetYUVCallBack");
    if(NULL == p_func_setYUVCallBack)
    {
        MessageBox(0, L"GetProcAddress SetYUVCallBack error", NULL, MB_OK);
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

    p_func_freeVideos = (ffreeVideos)GetProcAddress(hPlayH264DLL, "freeVideos");
    if(NULL == p_func_freeVideos)
    {
        MessageBox(0, L"GetProcAddress freeVideos error", NULL, MB_OK);
        exit(-1);
    }

    p_func_exitdll = (fexitdll)GetProcAddress(hPlayH264DLL, "exitdll");
    if(NULL == p_func_exitdll)
    {
        MessageBox(NULL, L"GetProcAddress exitdll error", NULL, MB_OK);
    }

    if(0 > p_func_initVideoDLL(max_client_number))
    {
        return -1;
    }

    return 0;
}

RTSPFFMPEG_API int FreeRtspDLL(void)
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

    p_func_exitdll();

    delete[] client_list;

    FreeLibrary(hPlayH264DLL);

    return 0;
}

RTSPFFMPEG_API int GetRtspINSTANCE(void)
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
                client_list[i].pt->m_p_func_freeVideos = p_func_freeVideos;
                client_list[i].pt->m_p_func_GetIdlevideoINSTANCE = p_func_GetIdlevideoINSTANCE;
                client_list[i].pt->m_p_func_InitVideoParam = p_func_InitVideoParam;
                client_list[i].pt->m_p_func_InitVideoParamNew = p_func_InitVideoParamNew;
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
                client_list[i].pt->m_p_func_setYUVCallBack = p_func_setYUVCallBack;
                return i;
            }
        }
    }
    return -1;
}

RTSPFFMPEG_API int InitRtspVideoParam(int INSTANCE, char* URI, char* userName, char* password)
{
    if(0 > checkINSTANCE(INSTANCE))
    {
        return -1;
    }

    if(NULL == client_list[INSTANCE].pt)
    {
        MessageBox(NULL, L"InitRtspVideoParam: new error!", NULL, MB_OK);
        return -1;
    }

    return client_list[INSTANCE].pt->input_URI(URI, userName, password);
}

RTSPFFMPEG_API int Connect(int INSTANCE)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == client_list[INSTANCE].pt)
    {
        return -1;
    }

    return client_list[INSTANCE].pt->connect();
}

RTSPFFMPEG_API int Play(int INSTANCE, HWND hWnd)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == client_list[INSTANCE].pt)
    {
        return -1;
    }

    return client_list[INSTANCE].pt->play(hWnd);
}

RTSPFFMPEG_API int Pause(int INSTANCE)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == client_list[INSTANCE].pt)
    {
        return -1;
    }

    return client_list[INSTANCE].pt->pause();
}

RTSPFFMPEG_API int DisConnect(int INSTANCE)
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

// set YUV callback function pointer
RTSPFFMPEG_API int SetYUV420CallBack(int INSTANCE, TYUVCallBack p_func_YUV420Func, void *buffer)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == p_func_YUV420Func)
    {
        return -1;
    }

    client_list[INSTANCE].pt->YUVFunc = p_func_YUV420Func;
    client_list[INSTANCE].pt->YUVEx = buffer;

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
