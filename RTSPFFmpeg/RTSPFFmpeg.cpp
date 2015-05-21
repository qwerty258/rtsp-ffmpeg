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
            (CRTSPCLient*)deList[i].pt->stopURL();
            delete deList[i].pt;
            deList[i].pt = NULL;
        }
    }
    return 0;
}

RTSPFFMPEG_API int GetRtspINSTANCE()
{
    for(int i = 0; i < MACPL; i++)
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
    try
    {
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        deList[INSTANCE].pt = new CRTSPCLient;
        Sleep(5);
        CRTSPCLient *DC = (CRTSPCLient*)deList[INSTANCE].pt;
        DC->InputURL(URI, UserName, PWD);
    }
    catch(...)
    {
        return -1;
    }
    return 0;
}

RTSPFFMPEG_API int PlayRtsp(int INSTANCE, HWND hd)
{
    try
    {
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        CRTSPCLient* DC = (CRTSPCLient*)deList[INSTANCE].pt;
        DC->PlayURL(hd);
    }
    catch(...)
    {
        return -1;
    }
    return 0;
}

RTSPFFMPEG_API int StopRtsp(int INSTANCE)
{
    try
    {
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        CRTSPCLient*DC = (CRTSPCLient*)deList[INSTANCE].pt;
        DC->stopURL();
        Sleep(20);
        deList[INSTANCE].idle = 0;
        delete DC;
        deList[INSTANCE].pt = NULL;
    }
    catch(...)
    {
        return -1;
    }
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

    try
    {
        if(f1 == NULL) { return -1; };
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        CRTSPCLient *DC = (CRTSPCLient *)deList[INSTANCE].pt;
        DC->funcD = f1;
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

RTSPFFMPEG_API int pSetPFCALLBACK(int INSTANCE, PFCALLBACK f1)
{

    try
    {
        if(f1 == NULL) { return -1; };
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        CRTSPCLient *DC = (CRTSPCLient *)deList[INSTANCE].pt;
        DC->func = f1;
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

RTSPFFMPEG_API int pSetBmpCallBack(int INSTANCE, TBmpCallBack f2)
{
    try
    {
        if(f2 == NULL) { return -1; };
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        CRTSPCLient *DC = (CRTSPCLient *)deList[INSTANCE].pt;
        DC->bmpFunc = f2;
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

RTSPFFMPEG_API int pSetFillBmpCallBack(int INSTANCE, TDrawRectCallBack f3)
{
    try
    {
        if(f3 == NULL) { return -1; };
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        CRTSPCLient *DC = (CRTSPCLient *)deList[INSTANCE].pt;
        DC->fillbmp = f3;
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

//
//获取YUV数据
RTSPFFMPEG_API int pSetYUVCallBack(int INSTANCE, TYUVCallBack f3, void *buffer)
{
    try
    {
        if(f3 == NULL) { return -1; };
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        CRTSPCLient *DC = (CRTSPCLient *)deList[INSTANCE].pt;
        DC->YUVFunc = f3;
        DC->YUVEx = buffer;
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}
//获取H264数据
RTSPFFMPEG_API int pSetH264CallBack(int INSTANCE, TH264CallBack f3)
{
    try
    {
        if(f3 == NULL) { return -1; };
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        CRTSPCLient *DC = (CRTSPCLient *)deList[INSTANCE].pt;
        DC->H264Func = f3;
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}
//设置是否取消硬解
RTSPFFMPEG_API int RevoHWAcceleration(int INSTANCE)
{
    try
    {
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        CRTSPCLient *DC = (CRTSPCLient *)deList[INSTANCE].pt;
        DC->nHWAcceleration = false;
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}
