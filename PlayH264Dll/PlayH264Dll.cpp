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

int debug = 0;
typedef struct container_decode_data
{
    CDecode *pt;
    int idle; //1 enable 0 disable 2 lock
}decode_data;

decode_data* decode_list;
int          max_decode_number;


int checkINSTANCE(int INSTANCE)
{
    if(0 > INSTANCE || max_decode_number < INSTANCE || NULL == decode_list || NULL == decode_list[INSTANCE].pt)
    {
        return -1;
    }
    return 0;
}

PLAYH264DLL_API int initVideoDLL(int max_number_of_playing_instance)
{
    if(0 >= max_number_of_playing_instance)
    {
        return -1;
    }

    av_register_all();

    max_decode_number = max_number_of_playing_instance;

    decode_list = new decode_data[max_decode_number];
    if(NULL == decode_list)
    {
        MessageBox(NULL, L"memory new error", NULL, MB_OK);
    }

    memset(decode_list, 0x0, max_decode_number * sizeof(decode_data));

    for(int i = 0; i < max_decode_number; i++)
    {
        decode_list[i].idle = 0;
        decode_list[i].pt = new CDecode;
    }

    return 0;
}

PLAYH264DLL_API void exitdll()
{
    for(int i = 0; i < max_decode_number; i++)
    {
        if(0 != decode_list[i].idle)
        {
            decode_list[i].idle = 0;
        }
        if(NULL != decode_list[i].pt)
        {
            decode_list[i].pt->paramUser.stopPlay = -2;
            delete decode_list[i].pt;
        }
    }
}

PLAYH264DLL_API int SetCallBack(int INSTANCE, PFCALLBACK f1) //depreated
{
    if(NULL == f1 || 0 > checkINSTANCE(INSTANCE))
    {
        return -1;
    }
    decode_list[INSTANCE].pt->func = f1;
    return 0;
}

PLAYH264DLL_API void DecodeVideo(int num, uint8_t * pInBuffer, int size)
{
    if(0 > checkINSTANCE(num) || 1 != decode_list[num].idle || NULL == decode_list[num].pt->m_parser)
    {
        return;
    }

    int pos = 0;
    int64_t pts = AV_NOPTS_VALUE;
    int64_t dts = AV_NOPTS_VALUE;
    //   CDecode *DC=(CDecode *)deList[num].pt;
    do
    {
        uint8_t *pout;// a complete frame, if incomplete it's NULL
        int pout_len;// a complete frame's length, if incomplete it's NULL
        int len = av_parser_parse2(decode_list[num].pt->m_parser, decode_list[num].pt->m_pCodecContext, &pout, &pout_len, pInBuffer + pos, size - pos, pts, dts, AV_NOPTS_VALUE);


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
            CDecode *DC = (CDecode *)decode_list[num].pt;
            if(DC == NULL) return;
            if(decode_list[num].idle == 1)
                DC->writeNetBuf(num, pout, pout_len);
        }
    } while(pos < size);


    //if(size<=0)
    //{
    //     //   CDecode *DC=(CDecode *)deList[num].pt;
    //     //   while(DC->writeNetBuf(num,NULL,0));
    //}
}

PLAYH264DLL_API int GetIdlevideoINSTANCE()
{
    if(debug == 1)
                WriteLog("C:\\1.log", "GetIdlevideoINSTANCE");
    for(int i = 0; i < max_decode_number; i++)
    {
        if(decode_list[i].idle == 0)
        {
            int ru = checkINSTANCE(i);
            if(ru == 0)
            {
                decode_list[i].idle = 2;
                decode_list[i].pt->INSTANCE = i;
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

    if(NULL == Myparam || 0 > checkINSTANCE(INSTANCE))
    {
        return -1;
    }
    //deList[INSTANCE].idle=2;

    decode_list[INSTANCE].pt->type = type;
    decode_list[INSTANCE].pt->nHWAcceleration = false;
    decode_list[INSTANCE].pt->bmpFunc = NULL;
    decode_list[INSTANCE].pt->fillbmp = NULL;
    decode_list[INSTANCE].pt->func = NULL;
    decode_list[INSTANCE].pt->funcD = NULL;
    decode_list[INSTANCE].pt->yuvFunc = NULL;
    decode_list[INSTANCE].pt->H264Func = NULL;

    //FILE *fp;
    //fp = fopen("c:\\set.txt","a+");
    //fputs("NULL",fp);
    //fclose(fp);

    if(1 == decode_list[INSTANCE].pt->type)
    {
        AVCodec *codec;
        codec = avcodec_find_decoder(CODEC_ID_H264);
        decode_list[INSTANCE].pt->m_pCodecContext = avcodec_alloc_context3(codec);
        decode_list[INSTANCE].pt->m_pFrame = avcodec_alloc_frame();
        decode_list[INSTANCE].pt->m_parser = av_parser_init(CODEC_ID_H264);
    }
    if(2 == decode_list[INSTANCE].pt->type)
    {
        AVCodec *codec;
        codec = avcodec_find_decoder(CODEC_ID_MPEG4);
        decode_list[INSTANCE].pt->m_pCodecContext = avcodec_alloc_context3(codec);
        decode_list[INSTANCE].pt->m_pFrame = avcodec_alloc_frame();
        decode_list[INSTANCE].pt->m_parser = av_parser_init(CODEC_ID_MPEG4);
    }
    decode_list[INSTANCE].idle = 1;
    if(debug == 1)
            WriteLog("C:\\1.log", "InitVideoParam2");

    return decode_list[INSTANCE].pt->InputParam(Myparam);
    //f1=fopen("c:\\buf264.h264","wb");
}

PLAYH264DLL_API int pauseVideos(int INSTANCE)
{
    if(0 > checkINSTANCE(INSTANCE))
    {
        return -1;
    }
    decode_list[INSTANCE].pt->pauseVideo();
    return 0;
}

PLAYH264DLL_API int playVideos(int INSTANCE)
{
    if(debug == 1)
            WriteLog("C:\\1.log", "playVideos");
    if(0 > checkINSTANCE(INSTANCE))
    {
        return -1;
    }
    decode_list[INSTANCE].pt->playVideo();
    return 0;
}

PLAYH264DLL_API int freeVideos(int INSTANCE)
{
    if(debug == 1)
             WriteLog("C:\\1.log", "freeVideos");
    if(0 > checkINSTANCE(INSTANCE) || 1 != decode_list[INSTANCE].idle)
    {
        return -1;
    }
    decode_list[INSTANCE].idle = 2;//ensure locks
    decode_list[INSTANCE].pt->freeParam();
    if(debug == 1)
              WriteLog("C:\\1.log", "freeVideos1");
    //m_pCodecContext[INSTANCE]=avcodec_alloc_context3(codec);
    //   m_pFrame[INSTANCE]=avcodec_alloc_frame();
    //   m_parser[INSTANCE]=av_parser_init(CODEC_ID_H264);
    avcodec_close(decode_list[INSTANCE].pt->m_pCodecContext);
    // avcodec_free_context(&m_pCodecContext[INSTANCE]);
    av_freep(&decode_list[INSTANCE].pt->m_pCodecContext);
    if(debug == 1)
              WriteLog("C:\\1.log", "freeVideos2");
    //av_free(m_pFrame[INSTANCE]->data[0]);
    av_frame_free(&decode_list[INSTANCE].pt->m_pFrame);
    if(debug == 1)
              WriteLog("C:\\1.log", "freeVideos3");

    av_parser_close(decode_list[INSTANCE].pt->m_parser);
    decode_list[INSTANCE].pt->m_pCodecContext = NULL;
    decode_list[INSTANCE].pt->m_pFrame = NULL;
    decode_list[INSTANCE].pt->m_parser = NULL;
    //if(debug == 1)
    //          WriteLog("C:\\1.log", "freeVideos4");

    decode_list[INSTANCE].idle = 0;// release lock

    //FILE *fp;
    //fp = fopen("c:\\free.txt","a+");
    //fputs("free",fp);
    //fclose(fp);

    //Sleep(1);
    return 0;
}

PLAYH264DLL_API int inputBuf(int INSTANCE, char *buf, int bufsize)
{
    // 	FILE *fp;
    //fp = fopen("c:\\20150205.h264","ab+");
    //fwrite(buf,1,bufsize,fp);
    //fclose(fp);

    if(debug == 1)
       WriteLog("C:\\1.log", "inputBuf");
    if(0 > checkINSTANCE(INSTANCE) || 1 != decode_list[INSTANCE].idle || NULL == buf || 0 > bufsize)
    {
        return -1;
    }
    decode_list[INSTANCE].pt->writeNetBuf(INSTANCE, (unsigned char*)buf, bufsize);
    if(debug == 1)
      WriteLog("C:\\1.log", "inputBufEnd");
    return 0;
}

PLAYH264DLL_API int resize(int INSTANCE, int width, int height)
{
    if(0 > checkINSTANCE(INSTANCE))
    {
        return -1;
    }
    decode_list[INSTANCE].pt->playResize(width, height);
    return 0;
}

PLAYH264DLL_API int SetDrawLineCallBack(int INSTANCE, TDrawLineCallBack f1)//depreated
{
    //FILE *fp;
    //fp = fopen("c:\\set.txt","a+");
    //fputs("set",fp);
    //fclose(fp);
    if(NULL == f1 || 0 > checkINSTANCE(INSTANCE))
    {
        return -1;
    }
    decode_list[INSTANCE].pt->funcD = f1;
    return 0;
}

PLAYH264DLL_API int SetBmpCallBack(int INSTANCE, TBmpCallBack bmp1)//depreated
{
    if(NULL == bmp1 || 0 > checkINSTANCE(INSTANCE))
    {
        return -1;
    }
    decode_list[INSTANCE].pt->bmpFunc = bmp1;
    return 0;
}

PLAYH264DLL_API int SetFillBmpCallBack(int INSTANCE, TDrawRectCallBack bmpf)//depreated
{
    if(NULL == bmpf || 0 > checkINSTANCE(INSTANCE))
    {
        return -1;
    }
    decode_list[INSTANCE].pt->fillbmp = bmpf;
    return 0;
}

PLAYH264DLL_API int SetYUVCallBack(int INSTANCE, TYUVCallBack yuvf, void *buffer)
{
    if(NULL == yuvf || 0 > checkINSTANCE(INSTANCE))
    {
        return -1;
    }
    decode_list[INSTANCE].pt->yuvFunc = yuvf;
    decode_list[INSTANCE].pt->userBuffer = buffer;
    return 0;
}

PLAYH264DLL_API int SetH264CallBack(int INSTANCE, TH264CallBack yuvf)
{
    if(NULL == yuvf || 0 > checkINSTANCE(INSTANCE))
    {
        return -1;
    }
    decode_list[INSTANCE].pt->H264Func = yuvf;
    return 0;
}

//support hardware acceleration, default is software decode.
//hardware acceleration support h264 and YUV callback, software decode support all callback functions
PLAYH264DLL_API int RevoHWAcceleration(int instance)
{
    if(0 > checkINSTANCE(instance))
    {
        return -1;
    }
    decode_list[instance].pt->nHWAcceleration = true;
    return 0;
}