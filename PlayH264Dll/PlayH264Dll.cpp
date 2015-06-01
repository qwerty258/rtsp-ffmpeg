// playH264ThreadDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "PlayH264Dll.h"
#include <fstream>
#include  "Gunit.h"
#pragma warning(disable: 4996)

using namespace std;

#ifdef _DEBUG // for memory leak check
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif // _DEBUG

#define MACPL 100
int debug = 0;
struct DecodeList
{
    playH264VideoClass *pt;
    CRITICAL_SECTION cs;
    int idle; //1 enable 0 disable 2 lock
};

AVCodecContext* m_pCodecContext[MACPL];
AVFrame * m_pFrame[MACPL];
AVCodecParserContext * m_parser[MACPL];

DecodeList deList[MACPL];




int checkINSTANCE(int INSTANCE)
{
    try
    {
        if((INSTANCE<0) || (INSTANCE>MACPL)) return -1;
        if(deList == NULL) return -1;
        if(deList[INSTANCE].pt == NULL) return -1;
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

PLAYH264DLL_API int SetCallBack(int INSTANCE, PFCALLBACK f1)//depreated
{
    try
    {
        if(f1 == NULL)
        {
            return -1;
        };
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        playH264VideoClass *DC = (playH264VideoClass *)deList[INSTANCE].pt;
        DC->func = f1;
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

PLAYH264DLL_API void DecodeVideo(int num, uint8_t * pInBuffer, int size)
{


    int ru = checkINSTANCE(num);
    if(ru < 0) return;
    if(deList[num].idle != 1) return;
    if(!m_parser)
        return;

    int pos = 0;
    int64_t pts = AV_NOPTS_VALUE;
    int64_t dts = AV_NOPTS_VALUE;
    //   playH264VideoClass *DC=(playH264VideoClass *)deList[num].pt;
    do
    {
        uint8_t *pout;// a complete frame, if incomplete it's NULL
        int pout_len;// a complete frame's length, if incomplete it's NULL
        int len = av_parser_parse2(m_parser[num], m_pCodecContext[num], &pout, &pout_len,
            pInBuffer + pos, size - pos, pts, dts, AV_NOPTS_VALUE);


        pos += len;


        if(pout_len > 0)
        {
            /*
            while(((*(pout+pout_len-1)==0xFE)||(*(pout+pout_len-1)==0xFD)||(*(pout+pout_len-1)==0xFF)||(*(pout+pout_len-1)==0x7E)||(*(pout+pout_len-1)==0x7D)||(*(pout+pout_len-1)==0x7C)||(*(pout+pout_len-1)==0xFC)||(*(pout+pout_len-1)==0xFB)||(*(pout+pout_len-1)==0x7B))&&((*(pout+pout_len-320)==0xFE)||(*(pout+pout_len-320)==0xFD)||(*(pout+pout_len-320)==0xFF)||(*(pout+pout_len-320)==0x7E)||(*(pout+pout_len-320)==0x7D)||(*(pout+pout_len-320)==0x7C)||(*(pout+pout_len-320)==0xFC)||(*(pout+pout_len-320)==0xFB)||(*(pout+pout_len-320)==0x7B))&&((*(pout+pout_len-160)==0xFE)||(*(pout+pout_len-160)==0xFD)||(*(pout+pout_len-160)==0xFF)||(*(pout+pout_len-160)==0x7E)||(*(pout+pout_len-160)==0x7D)||(*(pout+pout_len-160)==0x7C)||(*(pout+pout_len-160)==0xFC)||(*(pout+pout_len-160)==0xFB)||(*(pout+pout_len-160)==0x7B)))
            {
            pout_len-=320;
            }

            if (debug==1)
            WriteLog("C:\\1.log","decoeVideo2");
            */
            playH264VideoClass *DC = (playH264VideoClass *)deList[num].pt;
            if(DC == NULL) return;
            if(deList[num].idle == 1)
                DC->writeNetBuf(num, pout, pout_len);
        }
    } while(pos < size);


    //if(size<=0)
    //{
    //     //   playH264VideoClass *DC=(playH264VideoClass *)deList[num].pt;
    //     //   while(DC->writeNetBuf(num,NULL,0));
    //}
}

PLAYH264DLL_API int initVideoDLL()
{
    //  	char sdl_var[64];
    //sprintf(sdl_var, "SDL_WINDOWID=%d", hd);
    //SDL_putenv(sdl_var);
    /*FILE *fp;
    fp = fopen("c:\\20150205.txt","a+");
    fputs("initial success",fp);
    fclose(fp);*/

    if(debug == 1)
    WriteLog("C:\\1.log", "initVideoDLL");
    //InitializeCriticalSection( & cs);
    avcodec_register_all();
    av_register_all();

    //	avcodec_register_all();
    //  av_register_all();
    //     for (int i=0;i<2;i++)
    //     {
    //         m_pCodecContext[i]=avcodec_alloc_context();
    //         m_pFrame[i]=avcodec_alloc_frame();
    //         m_parser[i]=av_parser_init(CODEC_ID_H264);
    //     }
    //en or de?
    //for(int i=0;i<MACPL;i++)
    //  {
    //     AVCodec *codec;   
    //  codec = avcodec_find_decoder(CODEC_ID_H264);
    //  m_pCodecContext[i]=avcodec_alloc_context3(codec);
    //     m_pFrame[i]=avcodec_alloc_frame();
    //     m_parser[i]=av_parser_init(CODEC_ID_H264);
    //  }

    for(int i = 0; i < MACPL; i++)
    {
        deList[i].idle = 0;
        deList[i].pt = new playH264VideoClass;
        deList[i].cs = {0};
    }

    return 0;
}

PLAYH264DLL_API int GetIdlevideoINSTANCE()
{
    if(debug == 1)
                WriteLog("C:\\1.log", "GetIdlevideoINSTANCE");
    for(int i = 0; i < MACPL; i++)
    {
        if(deList[i].idle == 0)
        {
            int ru = checkINSTANCE(i);
            if(ru == 0)
            {
                deList[i].idle = 2;
                playH264VideoClass *DC = (playH264VideoClass *)deList[i].pt;
                DC->INSTANCE = i;
                //DC->func=NULL;
                return i;
            }
        }
    }
    return -1;
}

PLAYH264DLL_API int InitVideoParam(int INSTANCE, myparamInput *Myparam, int type)
{
    if(debug == 1)
                WriteLog("C:\\1.log", "InitVideoParam1");

    if(Myparam == NULL)
    {
        return -1;
    };
    int ru = checkINSTANCE(INSTANCE);
    if(ru < 0) return -1;
    //deList[INSTANCE].idle=2;

    deList[INSTANCE].pt->type = type;
    deList[INSTANCE].pt->nHWAcceleration = false;
    deList[INSTANCE].pt->bmpFunc = NULL;
    deList[INSTANCE].pt->fillbmp = NULL;
    deList[INSTANCE].pt->func = NULL;
    deList[INSTANCE].pt->funcD = NULL;
    deList[INSTANCE].pt->yuvFunc = NULL;
    deList[INSTANCE].pt->H264Func = NULL;
    ru = deList[INSTANCE].pt->InputParam(Myparam);

    //FILE *fp;
    //fp = fopen("c:\\set.txt","a+");
    //fputs("NULL",fp);
    //fclose(fp);

    if(deList[INSTANCE].pt->type == 1)
    {
        AVCodec *codec;
        codec = avcodec_find_decoder(CODEC_ID_H264);
        m_pCodecContext[INSTANCE] = avcodec_alloc_context3(codec);
        m_pFrame[INSTANCE] = avcodec_alloc_frame();
        m_parser[INSTANCE] = av_parser_init(CODEC_ID_H264);
    }
    if(deList[INSTANCE].pt->type == 2)
    {
        AVCodec *codec;
        codec = avcodec_find_decoder(CODEC_ID_MPEG4);
        m_pCodecContext[INSTANCE] = avcodec_alloc_context3(codec);
        m_pFrame[INSTANCE] = avcodec_alloc_frame();
        m_parser[INSTANCE] = av_parser_init(CODEC_ID_MPEG4);
    }
    deList[INSTANCE].idle = 1;
    if(debug == 1)
            WriteLog("C:\\1.log", "InitVideoParam2");
    return ru;
    //f1=fopen("c:\\buf264.h264","wb");
}

PLAYH264DLL_API int pauseVideos(int INSTANCE)
{
    try
    {
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        playH264VideoClass *DC = (playH264VideoClass *)deList[INSTANCE].pt;
        DC->pauseVideo();
    }
    catch(...)
    {
        return -1;
    }
    return 0;
}

PLAYH264DLL_API int playVideos(int INSTANCE)
{
    if(debug == 1)
            WriteLog("C:\\1.log", "playVideos");
    try
    {
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        playH264VideoClass *DC = (playH264VideoClass *)deList[INSTANCE].pt;
        DC->playVideo();
    }
    catch(...)
    {
        return -1;
    }
    return 0;
}

PLAYH264DLL_API int freeVideos(int INSTANCE)
{
    try
    {
        if(debug == 1)
                 WriteLog("C:\\1.log", "freeVideos");
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        if(deList[INSTANCE].idle != 1) return -1;
        deList[INSTANCE].idle = 2;//ensure locks
        playH264VideoClass *DC = (playH264VideoClass *)deList[INSTANCE].pt;
        DC->freeParam();
        if(debug == 1)
                  WriteLog("C:\\1.log", "freeVideos1");
        //m_pCodecContext[INSTANCE]=avcodec_alloc_context3(codec);
        //   m_pFrame[INSTANCE]=avcodec_alloc_frame();
        //   m_parser[INSTANCE]=av_parser_init(CODEC_ID_H264);
        avcodec_close(m_pCodecContext[INSTANCE]);
        // avcodec_free_context(&m_pCodecContext[INSTANCE]);
        av_freep(&m_pCodecContext[INSTANCE]);
        if(debug == 1)
                  WriteLog("C:\\1.log", "freeVideos2");
        //av_free(m_pFrame[INSTANCE]->data[0]);
        av_frame_free(&m_pFrame[INSTANCE]);
        if(debug == 1)
                  WriteLog("C:\\1.log", "freeVideos3");

        av_parser_close(m_parser[INSTANCE]);
        m_pCodecContext[INSTANCE] = NULL;
        m_pFrame[INSTANCE] = NULL;
        m_parser[INSTANCE] = NULL;
        //if(debug == 1)
        //          WriteLog("C:\\1.log", "freeVideos4");

        deList[INSTANCE].idle = 0;// release lock

        //FILE *fp;
        //fp = fopen("c:\\free.txt","a+");
        //fputs("free",fp);
        //fclose(fp);

        //Sleep(1);
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

PLAYH264DLL_API int inputBuf(int INSTANCE, char *buf, int bufsize)
{
    // 	FILE *fp;
    //fp = fopen("c:\\20150205.h264","ab+");
    //fwrite(buf,1,bufsize,fp);
    //fclose(fp);

    try
    {
        if(debug == 1)
           WriteLog("C:\\1.log", "inputBuf");
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        if(deList[INSTANCE].idle != 1) return -1;
        if(buf == NULL) return -1;
        if(bufsize < 0) return -1;
        playH264VideoClass *DC = (playH264VideoClass *)deList[INSTANCE].pt;
        DC->writeNetBuf(INSTANCE, (unsigned char*)buf, bufsize);
        if(debug == 1)
          WriteLog("C:\\1.log", "inputBufEnd");
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

PLAYH264DLL_API int resize(int INSTANCE, int width, int height)
{
    try
    {
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        playH264VideoClass *DC = (playH264VideoClass *)deList[INSTANCE].pt;
        DC->playResize(width, height);
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

PLAYH264DLL_API void exitdll()
{
    for(int i = 0; i < MACPL; i++)
    {
        if(0 != deList[i].idle)
        {
            deList[i].idle = 0;
        }
        if(NULL != deList[i].pt)
        {
            deList[i].pt->paramUser.stopPlay = -2;
            delete deList[i].pt;
        }
    }
}

PLAYH264DLL_API int SetDrawLineCallBack(int INSTANCE, TDrawLineCallBack f1)//depreated
{

    //FILE *fp;
    //fp = fopen("c:\\set.txt","a+");
    //fputs("set",fp);
    //fclose(fp);
    try
    {
        if(f1 == NULL)
        {
            return -1;
        };
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0) return -1;
        playH264VideoClass *DC = (playH264VideoClass *)deList[INSTANCE].pt;
        DC->funcD = f1;
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

PLAYH264DLL_API int SetBmpCallBack(int INSTANCE, TBmpCallBack bmp1)//depreated
{
    try
    {
        if(bmp1 == NULL)
        {
            return -1;
        }
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0)
        {
            return -1;
        }
        playH264VideoClass *PC = (playH264VideoClass *)deList[INSTANCE].pt;
        PC->bmpFunc = bmp1;
        return 0;
    }
    catch(...)
    {
        return -1;
    }

}

PLAYH264DLL_API int SetFillBmpCallBack(int INSTANCE, TDrawRectCallBack bmpf)//depreated
{
    try
    {
        if(bmpf == NULL)
        {
            return -1;
        }
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0)
        {
            return -1;
        }
        playH264VideoClass *PC = (playH264VideoClass *)deList[INSTANCE].pt;
        PC->fillbmp = bmpf;
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

PLAYH264DLL_API int SetYUVCallBack(int INSTANCE, TYUVCallBack yuvf, void *buffer)
{
    try
    {
        if(yuvf == NULL)
        {
            return -1;
        }
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0)
        {
            return -1;
        }
        playH264VideoClass *PC = (playH264VideoClass *)deList[INSTANCE].pt;
        PC->yuvFunc = yuvf;
        PC->userBuffer = buffer;
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

PLAYH264DLL_API int SetH264CallBack(int INSTANCE, TH264CallBack yuvf)
{
    try
    {
        if(yuvf == NULL)
        {
            return -1;
        }
        int ru = checkINSTANCE(INSTANCE);
        if(ru < 0)
        {
            return -1;
        }
        playH264VideoClass *PC = (playH264VideoClass *)deList[INSTANCE].pt;
        PC->H264Func = yuvf;
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

//support hardware acceleration, default is software decode.
//hardware acceleration support h264 and YUV callback, software decode support all callback functions
PLAYH264DLL_API int RevoHWAcceleration(int instance)
{
    try
    {

        int ru = checkINSTANCE(instance);
        if(ru < 0)
        {
            return -1;
        }
        playH264VideoClass *PC = (playH264VideoClass *)deList[instance].pt;
        PC->nHWAcceleration = true;
        return 0;
    }
    catch(...)
    {
        return -1;
    }
}