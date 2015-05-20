#include "RTSPFFmpeg.h"

#define  MACPL 100
struct ClientList
{
    RTSPCLientClass *pt;
    int idle;
};
ClientList deList[MACPL];

int checkINSTANCE(int INSTANCE)
{
    try
    {
        if((INSTANCE<0) || (INSTANCE>MACPL)) return -1;
        if(deList == NULL) return -1;
        //if (deList[INSTANCE].pt==NULL) return -1;
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

RTSPFFMPEG_API int GetRtspINSTANCE()
{
#ifdef log
    FILE *fp;
    fp=fopen("c:\\test.log","ab+");
    fputs("GetRtspINSTANCE\n",fp);
    fclose(fp);
#endif
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

RTSPFFMPEG_API int InitRtspVideoParam(int INSTANCE, char* URL, char* UserName, char* PWD)
{
#ifdef log
    FILE *fp;
    fp=fopen("c:\\test.log","ab+");
    fputs("InitRtspVideo\n",fp);
    fclose(fp);
#endif
    try
    {
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        deList[INSTANCE].pt = new RTSPCLientClass;
        Sleep(5);
        RTSPCLientClass *DC = (RTSPCLientClass *)deList[INSTANCE].pt;
        DC->InputURL(URL, UserName, PWD);
    }
    catch(...)
    {
        return -1;
    }
    return 0;
}

RTSPFFMPEG_API int PlayRtsp(int INSTANCE, HWND hd)
{
#ifdef log
    FILE *fp;
    fp=fopen("c:\\test.log","ab+");
    fputs("PlayRtsp\n",fp);
    fclose(fp);
#endif
    try
    {
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        RTSPCLientClass *DC = (RTSPCLientClass *)deList[INSTANCE].pt;
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
        RTSPCLientClass *DC = (RTSPCLientClass *)deList[INSTANCE].pt;
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

RTSPFFMPEG_API int InitRtspDLL()
{
#ifdef log
    FILE *fp;
    fp=fopen("c:\\test.log","ab+");
    fputs("InitRtspDLL\n",fp);
    fclose(fp);
#endif
    for(int i = 0; i < MACPL; i++)
    {
        deList[i].idle = 0;
        deList[i].pt = NULL;
    }
    return 0;
}

RTSPFFMPEG_API int FreeRtspDLL(int INSTANCE)
{

    try
    {
        for(int i = 0; i < MACPL; i++)
        {
            if(deList[i].idle == 0)
            {
                RTSPCLientClass *DC = (RTSPCLientClass *)deList[INSTANCE].pt;
                DC->stopURL();
                delete DC;

            }
            else
            {
                RTSPCLientClass *DC = (RTSPCLientClass *)deList[INSTANCE].pt;
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
        RTSPCLientClass *DC = (RTSPCLientClass *)deList[INSTANCE].pt;
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
        RTSPCLientClass *DC = (RTSPCLientClass *)deList[INSTANCE].pt;
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
        RTSPCLientClass *DC = (RTSPCLientClass *)deList[INSTANCE].pt;
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
        RTSPCLientClass *DC = (RTSPCLientClass *)deList[INSTANCE].pt;
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
        RTSPCLientClass *DC = (RTSPCLientClass *)deList[INSTANCE].pt;
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
        RTSPCLientClass *DC = (RTSPCLientClass *)deList[INSTANCE].pt;
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
        RTSPCLientClass *DC = (RTSPCLientClass *)deList[INSTANCE].pt;
        DC->nHWAcceleration = false;
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}
