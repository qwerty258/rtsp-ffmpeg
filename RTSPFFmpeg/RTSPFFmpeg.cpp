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
    for(size_t i = 0; i < MACPL; ++i)
    {
        deList[i].idle = 0;
        deList[i].pt = NULL;
    }
    return 0;
}

RTSPFFMPEG_API int FreeRtspDLL()
{
    for(size_t i = 0; i < MACPL; ++i)
    {
        if(0 != deList[i].idle)
        {
            deList[i].idle = 0;
        }

        if(NULL != deList[i].pt)
        {
            ((CRTSPCLient*)deList[i].pt)->stopURL();
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
        if(deList[i].idle == 0)
        {
            int ru = checkINSTANCE(i);
            if(ru == 0)
            {
                deList[i].idle = 1;
                //deList[i].pt->fillbmp = NULL;
                //deList[i].pt->func = NULL;
                //deList[i].pt->funcD = NULL;
                //deList[i].pt->bmpFunc = NULL;
                return i;
            }
        }
    }
    return -1;
}

RTSPFFMPEG_API int InitRtspVideoParam(int INSTANCE, char* URI, char* UserName, char* PWD)
{
    if(checkINSTANCE(INSTANCE) < 0)
    {
        return -1;
    }

    deList[INSTANCE].pt = new CRTSPCLient;

    if(NULL == deList[INSTANCE].pt)
    {
        MessageBox(NULL, L"InitRtspVideoParam: new error!", NULL, MB_OK);
        return -1;
    }

    ((CRTSPCLient*)deList[INSTANCE].pt)->InputURL(URI, UserName, PWD);

    return 0;
}

RTSPFFMPEG_API int PlayRtsp(int INSTANCE, HWND hd)
{
    if(checkINSTANCE(INSTANCE) < 0)
    {
        return -1;
    }

    ((CRTSPCLient*)deList[INSTANCE].pt)->PlayURL(hd);

    return 0;
}

RTSPFFMPEG_API int StopRtsp(int INSTANCE)
{
    if(checkINSTANCE(INSTANCE) < 0)
    {
        return -1;
    }

    ((CRTSPCLient*)deList[INSTANCE].pt)->stopURL();

    deList[INSTANCE].idle = 0;

    delete deList[INSTANCE].pt;
    deList[INSTANCE].pt = NULL;

    return 0;
}


RTSPFFMPEG_API int FreeRtspDLL0000(int INSTANCE)
{

    try
    {
        for(int i = 0; i < MACPL; i++)
        {
            if(deList[i].idle == 0)
            {
                CRTSPCLient *DC = (CRTSPCLient *)deList[INSTANCE].pt;
                DC->stopURL();
                delete DC;

            }
            else
            {
                CRTSPCLient *DC = (CRTSPCLient *)deList[INSTANCE].pt;
                DC->stopURL();
                Sleep(40);
                deList[INSTANCE].idle = 0;
                delete DC;
            }
        }
        //int ru=checkINSTANCE(INSTANCE);
        //if (ru<0) return -1;
        //RTSPCLientClass *DC=(RTSPCLientClass *)deList[INSTANCE].pt;
        //delete DC;
        //Sleep(50);
        ///*deList[INSTANCE].pt = new RTSPCLientClass;
        //Sleep(50);
        //deList[INSTANCE].idle=0;*/
        return 0;
    }
    catch(...)
    {
        return -1;
    }
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

//
//获取YUV数据
RTSPFFMPEG_API int pSetYUVCallBack(int INSTANCE, TYUVCallBack f3, void *buffer)
{
    if(NULL == f3)
    {
        return -1;
    }

    if(checkINSTANCE(INSTANCE) < 0)
    {
        return -1;
    }

    ((CRTSPCLient*)deList[INSTANCE].pt)->YUVFunc = f3;
    ((CRTSPCLient*)deList[INSTANCE].pt)->YUVEx = buffer;

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
//设置是否取消硬解
RTSPFFMPEG_API int RevoHWAcceleration(int INSTANCE)
{
    if(checkINSTANCE(INSTANCE) < 0)
    {
        return -1;
    }

    ((CRTSPCLient *)deList[INSTANCE].pt)->nHWAcceleration = false;

    return 0;
}
