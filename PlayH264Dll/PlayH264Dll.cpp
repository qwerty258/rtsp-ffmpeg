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


typedef struct container_decode_data
{
    CDecode* p_CDecode;
    int idle; //1 enable 0 disable 2 lock
}decode_data;

decode_data* decode_list;
int          max_decode_number;


int check_instance(int instance)
{
    if(0 > instance || max_decode_number < instance || NULL == decode_list)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

PLAYH264DLL_API int initial_decode_DLL(int max_number_of_decoding_instance)
{
    if(0 >= max_number_of_decoding_instance)
    {
        return -1;
    }

    av_register_all();

    max_decode_number = max_number_of_decoding_instance;

    decode_list = new decode_data[max_decode_number];
    if(NULL == decode_list)
    {
        MessageBox(NULL, L"memory new error", NULL, MB_OK);
    }

    memset(decode_list, 0x0, max_decode_number * sizeof(decode_data));

    return 0;
}

PLAYH264DLL_API int free_decode_DLL(void)
{
    for(int i = 0; i < max_decode_number; i++)
    {
        if(0 != decode_list[i].idle)
        {
            decode_list[i].idle = 0;
        }
        if(NULL != decode_list[i].p_CDecode)
        {
            decode_list[i].p_CDecode->paramUser.stopPlay = -2;
            delete decode_list[i].p_CDecode;
        }
    }

    delete[] decode_list;

    return 0;
}

PLAYH264DLL_API int get_idle_instance(void)
{
    for(int i = 0; i < max_decode_number; ++i)
    {
        if(decode_list[i].idle == 0)
        {
            if(0 == check_instance(i))
            {
                decode_list[i].idle = 2;
                decode_list[i].p_CDecode = new CDecode;
                decode_list[i].p_CDecode->INSTANCE = i;
                return i;
            }
        }
    }

    return -1;
}

PLAYH264DLL_API int initial_decode_parameter(int instance, myparamInput* Myparam, int type)
{
    if(NULL == Myparam || 0 > check_instance(instance))
    {
        return -1;
    }

    decode_list[instance].p_CDecode->type = type;

    if(1 == decode_list[instance].p_CDecode->type)
    {
        AVCodec* codec = avcodec_find_decoder(CODEC_ID_H264);
        decode_list[instance].p_CDecode->m_pCodecContext = avcodec_alloc_context3(codec);
        decode_list[instance].p_CDecode->m_pFrame = avcodec_alloc_frame();
        decode_list[instance].p_CDecode->m_parser = av_parser_init(CODEC_ID_H264);
    }
    if(2 == decode_list[instance].p_CDecode->type)
    {
        AVCodec* codec = avcodec_find_decoder(CODEC_ID_MPEG4);
        decode_list[instance].p_CDecode->m_pCodecContext = avcodec_alloc_context3(codec);
        decode_list[instance].p_CDecode->m_pFrame = avcodec_alloc_frame();
        decode_list[instance].p_CDecode->m_parser = av_parser_init(CODEC_ID_MPEG4);
    }
    decode_list[instance].idle = 1;

    return decode_list[instance].p_CDecode->InputParam(Myparam);
}

PLAYH264DLL_API int decode(int instance, uint8_t* pInBuffer, int size)
{
    if(0 > check_instance(instance) || 1 != decode_list[instance].idle || NULL == decode_list[instance].p_CDecode->m_parser)
    {
        return -1;
    }

    int pos = 0;
    int64_t pts = AV_NOPTS_VALUE;
    int64_t dts = AV_NOPTS_VALUE;
    uint8_t *pout;// a complete frame, if incomplete it's NULL
    int pout_len;// a complete frame's length, if incomplete it's 0
    int len;

    do
    {
        len = av_parser_parse2(decode_list[instance].p_CDecode->m_parser, decode_list[instance].p_CDecode->m_pCodecContext, &pout, &pout_len, pInBuffer + pos, size - pos, pts, dts, AV_NOPTS_VALUE);

        pos += len;

        if(pout_len > 0)
        {
            if(decode_list[instance].idle == 1)
            {
                decode_list[instance].p_CDecode->writeNetBuf(instance, pout, pout_len);
            }
        }
    } while(pos < size);

    return 0;
}

PLAYH264DLL_API int free_decode_instance(int instance)
{
    if(0 > check_instance(instance) || 1 != decode_list[instance].idle)
    {
        return -1;
    }

    decode_list[instance].idle = 2;//ensure locks
    decode_list[instance].p_CDecode->freeParam();

    avcodec_close(decode_list[instance].p_CDecode->m_pCodecContext);

    av_freep(&decode_list[instance].p_CDecode->m_pCodecContext);

    av_frame_free(&decode_list[instance].p_CDecode->m_pFrame);

    av_parser_close(decode_list[instance].p_CDecode->m_parser);

    decode_list[instance].p_CDecode->m_pCodecContext = NULL;
    decode_list[instance].p_CDecode->m_pFrame = NULL;
    decode_list[instance].p_CDecode->m_parser = NULL;

    decode_list[instance].idle = 0;// release lock

    return 0;
}

PLAYH264DLL_API int SetCallBack(int instance, PFCALLBACK f1) //depreated
{
    if(NULL == f1 || 0 > check_instance(instance))
    {
        return -1;
    }
    decode_list[instance].p_CDecode->func = f1;
    return 0;
}

PLAYH264DLL_API int pauseVideos(int instance)
{
    if(0 > check_instance(instance))
    {
        return -1;
    }
    decode_list[instance].p_CDecode->pauseVideo();
    return 0;
}

PLAYH264DLL_API int playVideos(int instance)
{
    if(0 > check_instance(instance))
    {
        return -1;
    }
    decode_list[instance].p_CDecode->playVideo();
    return 0;
}

PLAYH264DLL_API int inputBuf(int instance, char *buf, int bufsize)
{
    // 	FILE *fp;
    //fp = fopen("c:\\20150205.h264","ab+");
    //fwrite(buf,1,bufsize,fp);
    //fclose(fp);

    if(0 > check_instance(instance) || 1 != decode_list[instance].idle || NULL == buf || 0 > bufsize)
    {
        return -1;
    }
    decode_list[instance].p_CDecode->writeNetBuf(instance, (unsigned char*)buf, bufsize);
    return 0;
}

PLAYH264DLL_API int resize(int instance, int width, int height)
{
    if(0 > check_instance(instance))
    {
        return -1;
    }
    decode_list[instance].p_CDecode->playResize(width, height);
    return 0;
}

PLAYH264DLL_API int SetDrawLineCallBack(int instance, TDrawLineCallBack f1)//depreated
{
    if(NULL == f1 || 0 > check_instance(instance))
    {
        return -1;
    }
    decode_list[instance].p_CDecode->funcD = f1;
    return 0;
}

PLAYH264DLL_API int SetBmpCallBack(int instance, TBmpCallBack bmp1)//depreated
{
    if(NULL == bmp1 || 0 > check_instance(instance))
    {
        return -1;
    }
    decode_list[instance].p_CDecode->bmpFunc = bmp1;
    return 0;
}

PLAYH264DLL_API int SetFillBmpCallBack(int instance, TDrawRectCallBack bmpf)//depreated
{
    if(NULL == bmpf || 0 > check_instance(instance))
    {
        return -1;
    }
    decode_list[instance].p_CDecode->fillbmp = bmpf;
    return 0;
}

PLAYH264DLL_API int SetYUVCallBack(int instance, TYUVCallBack yuvf, void *buffer)
{
    if(NULL == yuvf || 0 > check_instance(instance))
    {
        return -1;
    }
    decode_list[instance].p_CDecode->yuvFunc = yuvf;
    decode_list[instance].p_CDecode->userBuffer = buffer;
    return 0;
}

PLAYH264DLL_API int SetH264CallBack(int instance, TH264CallBack yuvf)
{
    if(NULL == yuvf || 0 > check_instance(instance))
    {
        return -1;
    }
    decode_list[instance].p_CDecode->H264Func = yuvf;
    return 0;
}

//support hardware acceleration, default is software decode.
//hardware acceleration support h264 and YUV callback, software decode support all callback functions
PLAYH264DLL_API int RevoHWAcceleration(int instance)
{
    if(0 > check_instance(instance))
    {
        return -1;
    }
    decode_list[instance].p_CDecode->nHWAcceleration = true;
    return 0;
}