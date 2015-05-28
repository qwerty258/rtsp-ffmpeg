#include "RTSPFFmpeg.h"

#define  MACPL 100
struct ClientList
{
    CRTSPCLient* pt;
    int idle;
};
ClientList deList[MACPL];

int checkINSTANCE(int INSTANCE)
{
    if(0 > INSTANCE || MACPL < INSTANCE || NULL == deList)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

RTSPFFMPEG_API int InitRtspDLL()
{
    for(int i = 0; i < MACPL; ++i)
    {
        deList[i].idle = 0;
        deList[i].pt = NULL;
    }
    return 0;
}

RTSPFFMPEG_API int FreeRtspDLL()
{
    for(int i = 0; i < MACPL; ++i)
    {
        if(0 != deList[i].idle)
        {
            deList[i].idle = 0;
        }

        if(NULL != deList[i].pt)
        {
            ((CRTSPCLient*)deList[i].pt)->disconnect();
            delete deList[i].pt;
            deList[i].pt = NULL;
        }
    }
    return 0;
}

RTSPFFMPEG_API int GetRtspINSTANCE()
{
    for(int i = 0; i < MACPL; ++i)
    {
        if(0 == deList[i].idle)
        {
            if(0 == checkINSTANCE(i))
            {
                deList[i].idle = 1;
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

    deList[INSTANCE].pt = new CRTSPCLient;

    if(NULL == deList[INSTANCE].pt)
    {
        MessageBox(NULL, L"InitRtspVideoParam: new error!", NULL, MB_OK);
        return -1;
    }

    return ((CRTSPCLient*)deList[INSTANCE].pt)->input_URI(URI, userName, password);
}

RTSPFFMPEG_API int Connect(int INSTANCE)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == deList[INSTANCE].pt)
    {
        return -1;
    }

    return ((CRTSPCLient*)deList[INSTANCE].pt)->connect();
}

RTSPFFMPEG_API int Play(int INSTANCE, HWND hWnd)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == deList[INSTANCE].pt)
    {
        return -1;
    }

    return ((CRTSPCLient*)deList[INSTANCE].pt)->play(hWnd);
}

RTSPFFMPEG_API int Pause(int INSTANCE)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == deList[INSTANCE].pt)
    {
        return -1;
    }

    return ((CRTSPCLient*)deList[INSTANCE].pt)->pause();
}

RTSPFFMPEG_API int DisConnect(int INSTANCE)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == deList[INSTANCE].pt)
    {
        return -1;
    }

    ((CRTSPCLient*)deList[INSTANCE].pt)->disconnect();

    deList[INSTANCE].idle = 0;

    delete deList[INSTANCE].pt;
    deList[INSTANCE].pt = NULL;

    return 0;
}

// set hardware acceleration
// acceleration: true: use hardware acceleration, false: use software decode
RTSPFFMPEG_API int RevoHWAcceleration(int INSTANCE, bool acceleration)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == deList[INSTANCE].pt)
    {
        return -1;
    }

    ((CRTSPCLient*)deList[INSTANCE].pt)->nHWAcceleration = acceleration;

    return 0;
}

// set YUV callback function pointer
RTSPFFMPEG_API int SetYUV420CallBack(int INSTANCE, YUV420Func p_func_YUV420Func, void *buffer)
{
    if(0 > checkINSTANCE(INSTANCE) || NULL == p_func_YUV420Func)
    {
        return -1;
    }

    ((CRTSPCLient*)deList[INSTANCE].pt)->YUVFunc = p_func_YUV420Func;
    ((CRTSPCLient*)deList[INSTANCE].pt)->YUVEx = buffer;

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

    ((CRTSPCLient*)deList[INSTANCE].pt)->funcD = f1;

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

    ((CRTSPCLient*)deList[INSTANCE].pt)->func = f1;

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

    ((CRTSPCLient*)deList[INSTANCE].pt)->bmpFunc = f2;

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

    ((CRTSPCLient*)deList[INSTANCE].pt)->fillbmp = f3;

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

    ((CRTSPCLient*)deList[INSTANCE].pt)->H264Func = f3;

    return 0;
}
