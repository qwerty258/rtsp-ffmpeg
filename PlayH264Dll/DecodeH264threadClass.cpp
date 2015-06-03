#include "stdafx.h"
#include "DecodeH264threadClass.h"
#include <atlimage.h>
#include <fstream>
#include  "Gunit.h"
#include <ddraw.h>
#include "va.h"
#include <d3d9.h>

using namespace std;
#pragma comment(lib,"ddraw.lib")
#pragma warning(disable: 4996)

#ifdef _DEBUG // for memory leak check
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif // _DEBUG

#define PICMAX (500000) //每一帧图片数据最大值
int playH264VideoClass::playBMPbuf(AVFrame *pFrameRGB, int width, int height, int playW, int playH, HDC playHD, HDC hmemDC, uint8_t *BMPbuf, HWND hWnd)
{
    bufptr = BMPbuf;
    if(fillbmp != NULL)
    {
        fillbmp((char*)bufptr, width, height);
    }
    if(func != NULL&&paramUser.isDecode)
    {
        memcpy(bufptr, &bmpheader, sizeof(bmpheader));
        memcpy(bufptr + sizeof(bmpheader), &bmpinfo, sizeof(bmpinfo));
        memcpy(bufptr + sizeof(bmpheader) + sizeof(bmpinfo), pFrameRGB->data[0], width*height*bpp / 8);
        func(this->INSTANCE, width, height, (char*)bufptr, sizeof(bmpheader) + sizeof(bmpinfo) + width*height*bpp / 8, BUFBMP);
    }

    hbit = CreateDIBitmap(playHD, (BITMAPINFOHEADER *)&bmpinfo, CBM_INIT, pFrameRGB->data[0], (BITMAPINFO *)&bmpinfo, DIB_RGB_COLORS);
    OldBitmap = (HBITMAP)SelectObject(hmemDC, hbit);
    if(funcD != NULL)
    {
        funcD(INSTANCE, hmemDC);
    }
    if(bmpFunc != NULL)
    {
        bmpFunc((char *)pFrameRGB->data[0], width*height * 3, width, height, 0, 0, 0, hWnd);
    }
    ::StretchBlt(playHD, 0, 0, playW, playH, hmemDC, 0, 0, width, height, SRCCOPY);
    SelectObject(hmemDC, OldBitmap);
    ::DeleteObject(hbit);
    ::DeleteObject(OldBitmap);
    return 0;
}


//////////////////////////////////////////////////////////////////
//功能：将buf写入netbuf链表，等待解码
//输入：buf 
//输出：写入netbuf链表
///////////////////////////////////////////////////////////////////
int playH264VideoClass::writeNetBuf(int num, unsigned char *buf, int bufsize)
{
    dataNode* p_data_node_temp = new dataNode;
    if(NULL == p_data_node_temp)
    {
        MessageBox(NULL, L"new memory error", NULL, MB_OK);
    }
    memset(p_data_node_temp, 0x0, sizeof(dataNode));

    p_data_node_temp->data = new unsigned char[bufsize];
    if(NULL == p_data_node_temp->data)
    {
        MessageBox(NULL, L"new memory error", NULL, MB_OK);
    }

    memcpy(p_data_node_temp->data, buf, bufsize);
    p_data_node_temp->size = bufsize;

    m_DataQueue.push(p_data_node_temp);

    // get memory usage and decide whether to throw away data

    GlobalMemoryStatusEx(&m_memory_statex);

    m_total_phys_memory = m_memory_statex.ullTotalPhys;
    m_available_phys_memory = m_memory_statex.ullAvailPhys;
    if(m_available_phys_memory / m_total_phys_memory < 0.1)
    {
        m_DataQueue.try_pop(p_data_node_temp);
        delete[] p_data_node_temp->data;
        delete p_data_node_temp;
        p_data_node_temp = NULL;
    }

    return 0;
}

int playH264VideoClass::setReadPosize(int index, int readsize)
{
    //try
    //{
    //  int itemCount=index;
    //  if (itemCount==0) {itemCount=ListCount;}else{itemCount--;};
    //  BuffList[itemCount]->readPos=readsize;
    //  return 0;
    //}catch(...)
    //{return -1;}
    return 0;
}
int playH264VideoClass::getNextNetBuf(char *buf, int bufsize)
{
    int size;
    dataNode* p_data_node_temp;
    try
    {
        //dataNode* p_temp_data_node;
        if(m_DataQueue.try_pop(p_data_node_temp))
        {
            if(STOPVIDEO == p_data_node_temp->size)
            {
                delete[] p_data_node_temp->data;
                delete p_data_node_temp;
                return STOPVIDEO;
            }
            else
            {
                memcpy(buf, p_data_node_temp->data, p_data_node_temp->size);
                size = p_data_node_temp->size;

                delete[] p_data_node_temp->data;
                delete p_data_node_temp;
                return size;
            }
        }
        else
        {
            return -1;
        }
    }
    catch(...)
    {
        return -1;
    }
}

int playH264VideoClass::InputParam(myparamInput *p1)
{
    try
    {
        if(p1 == NULL) return -1;
        paramUser.fps = p1->fps;//p1->fps;
        paramUser.playHandle = p1->playHandle;
        paramUser.playHeight = p1->playHeight;
        paramUser.playWidth = p1->playWidth;
        paramUser.stopPlay = p1->stopPlay;
        paramUser.playChannle = p1->playChannle;
        paramUser.isDecode = p1->isDecode;

        bmpheader.bfSize = bmpheader.bfOffBits + (p1->width)*(p1->height)*bpp / 8;
        bmpinfo.biSizeImage = ((p1->width)*bpp + 31) / 32 * 4 * (p1->height);

        bmpheader.bfType = 0x4d42;
        bmpheader.bfReserved1 = 0;
        bmpheader.bfReserved2 = 0;
        bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

        bmpinfo.biSize = sizeof(BITMAPINFOHEADER);

        bmpinfo.biPlanes = 1;
        bmpinfo.biBitCount = bpp;
        bmpinfo.biCompression = BI_RGB;

        bmpinfo.biXPelsPerMeter = 100;
        bmpinfo.biYPelsPerMeter = 100;
        bmpinfo.biClrUsed = 0;
        bmpinfo.biClrImportant = 0;
        hThreadDecode = CreateThread(NULL, 0, videoDecodeQueue, this, 0, &threadID);
        return threadID;
    }
    catch(...)
    {
        return STOPVIDEO;
    }

}

//////////////////////////////////////////////////////////////////
//功能：将buf写入netbuf链表，等待解码
//输入：buf 
//输出：写入链表
///////////////////////////////////////////////////////////////////
int playH264VideoClass::freeParam(void)
{
    try
    {
        paramUser.stopPlay = STOPVIDEO;
        dataNode * m_dataNode = new dataNode;
        m_dataNode->size = STOPVIDEO;
        m_dataNode->data = NULL;
        m_DataQueue.push(m_dataNode);
    }
    catch(...)
    {
        return -1;
    }
}

//////////////////////////////////////////////////////////////////
//功能：将buf写入netbuf链表，等待解码
//输入：buf 
//输出：写入链表
///////////////////////////////////////////////////////////////////
int playH264VideoClass::playVideo()
{
    paramUser.stopPlay = 0;
    return 0;
}
//////////////////////////////////////////////////////////////////
//功能：将buf写入netbuf链表，等待解码
//输入：buf 
//输出：写入链表
///////////////////////////////////////////////////////////////////
int playH264VideoClass::pauseVideo()
{
    paramUser.stopPlay = 0;
    return 0;
}
int playH264VideoClass::playResize(int newWidth, int newHeight)
{
    paramUser.playWidth = newWidth;
    paramUser.playHeight = newHeight;
    return 0;
}
playH264VideoClass::playH264VideoClass()
{
    //for(int i=0;i<ListCount;i++)
    //{
    //        BuffList[i]=new netBuf;
    //	 BuffList[i]->fileSize=0;
    //	 BuffList[i]->FrameNum=0;
    //	 BuffList[i]->readPos=0;
    //}
    readNetBufIndex = 0;
    writeNetBufIndex = 0;
    bpp = 24;//24色
    //writewait=CreateEvent(NULL,TRUE,FALSE,NULL);
    nHWAcceleration = false;
}

playH264VideoClass::~playH264VideoClass()
{
    //for(int i=0;i<ListCount;i++)
    //{
    //        delete BuffList[i]; 
    //	 //BuffList[i]=NULL;
    //	 //CloseHandle(hMutex);
    //}
}
//void WriteLog(char * LogFileName,int writeIndex,int readIndex)
//{
//	FILE *f;
//	f=fopen(LogFileName,"a+");
//	if(f==NULL) return ;
//	//
//	fprintf(f,"writeIndex=%d readIndex=%d\n",writeIndex,readIndex);
//	fclose(f);
//}

//int id = 0;

//关于GPU的全局变量
bool init = false;
LPDIRECTDRAW lpDD = NULL;// DirectDraw 对象指针
DDSURFACEDESC ddsd; // DirectDraw 表面描述
LPDIRECTDRAWSURFACE lpPrimary = NULL;// DirectDraw 主表面指针

//硬件加速模块
extern enum PixelFormat DxGetFormat(AVCodecContext *avctx, const enum PixelFormat *pi_fmt);
extern int DxGetFrameBuf(struct AVCodecContext *avctx, AVFrame *pic);
extern int  DxReGetFrameBuf(struct AVCodecContext *avctx, AVFrame *pic);
extern void DxReleaseFrameBuf(struct AVCodecContext *avctx, AVFrame *pic);
extern int DxPictureCopy(struct AVCodecContext *avctx, AVFrame *src, AVFrame* dst, void *);
int mAVCodecContextInit(AVCodecContext *avct)
{

    avct->get_buffer = DxGetFrameBuf;
    avct->reget_buffer = DxReGetFrameBuf;
    avct->release_buffer = DxReleaseFrameBuf;
    avct->opaque = NULL;
    //能否硬解
    if(avct->codec_id == CODEC_ID_MPEG1VIDEO || avct->codec_id == CODEC_ID_MPEG2VIDEO ||
            //avctx->codec_id == CODEC_ID_MPEG4 ||
            avct->codec_id == CODEC_ID_H264 ||
            avct->codec_id == CODEC_ID_VC1 || avct->codec_id == CODEC_ID_WMV3)
    {
        avct->get_format = DxGetFormat;
    }
    return 0;
    /*D3DXSaveSurfaceToFile = NULL;
      hdll = LoadLibrary(TEXT("D3DX9_42.DLL"));
      if(hdll)
      D3DXSaveSurfaceToFile =  (void *)GetProcAddress(hdll,TEXT("D3DXSaveSurfaceToFileA"));*/
}

//


//效率GDI版本
HWND gPlayWnd = NULL;
int bGPlayWnd = 0;
DWORD WINAPI videoDecodeQueue(LPVOID lpParam)
{
    bool fir = TRUE;
    int height = 0;
    int width = 0;
    //启用编码器 
    AVPacket avp;

    AVCodec* codec;
    AVCodecContext* cocec_context = NULL;
    int bufsize, len;
    int got_picture;
    AVFrame *picture, *picRGB;
    AVFrame *pFrameYUV = NULL;

    // av_register_all();
    int PictureSize;
    uint8_t *buf = NULL;
    char *netBuf = new char[PICMAX];
    char *bmpBuf = new char[1920 * 1080 * 3];

    if(NULL == netBuf || NULL == bmpBuf)
    {
        return -1;
    }

    HWND hd = ((playH264VideoClass*)lpParam)->paramUser.playHandle;

    HDC m_hdc = GetDC(hd);
    HDC hmemDC = CreateCompatibleDC(m_hdc);
    if(NULL == hmemDC)
    {
        return 0;
    }

    picture = avcodec_alloc_frame();
    picRGB = avcodec_alloc_frame();

    pFrameYUV = avcodec_alloc_frame();
    AVCodecID codeType;
    if(((playH264VideoClass*)lpParam)->type == 2)
        codeType = CODEC_ID_MPEG4;
    if(((playH264VideoClass*)lpParam)->type == 1)
        codeType = CODEC_ID_H264;
    if(((playH264VideoClass*)lpParam)->type == 3)
        codeType = CODEC_ID_FLV1;


    codec = avcodec_find_decoder(codeType);
    cocec_context = avcodec_alloc_context3(codec);

    if(((playH264VideoClass*)lpParam)->nHWAcceleration)
        mAVCodecContextInit(cocec_context);

    if(avcodec_open2(cocec_context, codec, NULL) < 0)
    {
        return 0;
    }

    extern int availableGPU[8];
    extern int currentGPU;

    struct SwsContext *pSwsCtx = NULL;

    for(;;)
    {
        bufsize = ((playH264VideoClass*)lpParam)->getNextNetBuf(netBuf, PICMAX);

        if(bufsize == STOPVIDEO)
        {
            break;
        }

        if(0 >= bufsize)
        {
            Sleep(30);
            continue;
        }

        av_init_packet(&avp);
        avp.data = (uint8_t *)netBuf;
        avp.size = bufsize;

        //h264输出
        int nWH = 0;
        if(((playH264VideoClass*)lpParam)->H264Func&&nWH)
        {
            ((playH264VideoClass*)lpParam)->H264Func(((playH264VideoClass*)lpParam)->INSTANCE, (char *)avp.data, avp.size, picture->width, picture->height);
            continue;
        }

        try
        {
            if(fir && ((playH264VideoClass*)lpParam)->nHWAcceleration)
            {
                while(bGPlayWnd)
                    Sleep(1);
                bGPlayWnd = 1;
                gPlayWnd = hd;

            }


            len = avcodec_decode_video2(cocec_context, picture, &got_picture, &avp);

            if(len < 0)
            {
                break;
            }

            if(fir && ((playH264VideoClass*)lpParam)->nHWAcceleration)
            {
                //GT620总值1000
                //1080,GPU消耗300
                //960,GPU 250
                //720,GPU 140
                //D1，GPU 60

                if(picture->height >= 1080)
                    availableGPU[currentGPU] += 300;

                else if(picture->height >= 960)
                    availableGPU[currentGPU] += 250;

                else if(picture->height >= 720)
                    availableGPU[currentGPU] += 140;

                else
                    availableGPU[currentGPU] += 60;

                if(((playH264VideoClass*)lpParam)->yuvFunc)
                {
                    pFrameYUV->width = picture->width;
                    pFrameYUV->height = picture->height;
                    int numBytes = avpicture_get_size(PIX_FMT_YUV420P, picture->width, picture->height);
                    avpicture_fill((AVPicture *)pFrameYUV, (uint8_t *)av_malloc(numBytes), PIX_FMT_YUV420P, picture->width, picture->height);
                }
                bGPlayWnd = 0;//打开开关
                fir = false;
            }
            if(!cocec_context->opaque && ((playH264VideoClass*)lpParam)->nHWAcceleration)//p_va失败
            {
                continue;
            }
            //h264输出
            if(((playH264VideoClass*)lpParam)->H264Func)
            {
                ((playH264VideoClass*)lpParam)->H264Func(((playH264VideoClass*)lpParam)->INSTANCE, (char *)avp.data, avp.size, picture->width, picture->height);
                nWH = 1;
                continue;
            }
            if(((playH264VideoClass*)lpParam)->nHWAcceleration)
                DxPictureCopy(cocec_context, picture, pFrameYUV, ((playH264VideoClass*)lpParam)->yuvFunc);//内部代码改为直接显示



            if(((playH264VideoClass*)lpParam)->yuvFunc != NULL && ((playH264VideoClass*)lpParam)->nHWAcceleration)
            {
                unsigned char * data = new unsigned char[2000 * 1500 / 4 * 6];
                for(int i = 0; i < cocec_context->height; i++)
                {
                    memcpy(data + i*cocec_context->width, pFrameYUV->data[0] + i*pFrameYUV->linesize[0], cocec_context->width);
                }
                for(int i = 0; i < cocec_context->height / 2; i++)
                {
                    memcpy(data + cocec_context->width*cocec_context->height + i*cocec_context->width / 2, pFrameYUV->data[2] + i*pFrameYUV->linesize[2], cocec_context->width / 2);
                }
                for(int i = 0; i < cocec_context->height / 2; i++)
                {
                    memcpy(data + cocec_context->width*cocec_context->height / 4 * 5 + i*cocec_context->width / 2, pFrameYUV->data[1] + i*pFrameYUV->linesize[1], cocec_context->width / 2);
                }
                ((playH264VideoClass*)lpParam)->yuvFunc(data, cocec_context->height, cocec_context->width, ((playH264VideoClass*)lpParam)->userBuffer);
                delete[] data;
            }
            if(((playH264VideoClass*)lpParam)->yuvFunc != NULL&&!((playH264VideoClass*)lpParam)->nHWAcceleration)
            {
                unsigned char * data = new unsigned char[2000 * 1500 / 4 * 6];
                for(int i = 0; i < cocec_context->height; i++)
                {
                    memcpy(data + i*cocec_context->width, picture->data[0] + i*picture->linesize[0],/*cocec_context->width*/1280);
                }
                for(int i = 0; i < cocec_context->height / 2; i++)
                {
                    memcpy(data + cocec_context->width*cocec_context->height + i*cocec_context->width / 2, picture->data[2] + i*picture->linesize[2], 640/*cocec_context->width/2*/);
                }
                for(int i = 0; i < cocec_context->height / 2; i++)
                {
                    memcpy(data + cocec_context->width*cocec_context->height / 4 * 5 + i*cocec_context->width / 2, picture->data[1] + i*picture->linesize[1], 640/*cocec_context->width/2*/);
                }
                ((playH264VideoClass*)lpParam)->yuvFunc(data, cocec_context->width, cocec_context->height, ((playH264VideoClass*)lpParam)->userBuffer);
                delete[] data;
            }

            if(fir&&!((playH264VideoClass*)lpParam)->nHWAcceleration)
            {
                width = cocec_context->width;
                height = cocec_context->height;
                PictureSize = avpicture_get_size(PIX_FMT_BGR24, cocec_context->width, cocec_context->height);
                buf = (uint8_t*)av_malloc(PictureSize);
                if(buf == NULL)
                {
                    break;
                }
                ((playH264VideoClass*)lpParam)->bmpinfo.biWidth = cocec_context->width;
                ((playH264VideoClass*)lpParam)->bmpinfo.biHeight = cocec_context->height;
                avpicture_fill((AVPicture *)picRGB, buf, PIX_FMT_BGR24, cocec_context->width, cocec_context->height);
            }

            if((width != cocec_context->width) || (height != cocec_context->height) && !((playH264VideoClass*)lpParam)->nHWAcceleration)
            {
                av_free(buf);
                width = cocec_context->width;
                height = cocec_context->height;
                PictureSize = avpicture_get_size(PIX_FMT_BGR24, cocec_context->width, cocec_context->height);
                buf = (uint8_t*)av_malloc(PictureSize);
                if(buf == NULL)
                {
                    break;
                }
                ((playH264VideoClass*)lpParam)->bmpinfo.biWidth = cocec_context->width;
                ((playH264VideoClass*)lpParam)->bmpinfo.biHeight = cocec_context->height;
                avpicture_fill((AVPicture *)picRGB, buf, PIX_FMT_BGR24, cocec_context->width, cocec_context->height);
            }

            if(!((playH264VideoClass*)lpParam)->nHWAcceleration&&fir)
            {
                pSwsCtx = sws_getContext(cocec_context->width, cocec_context->height, cocec_context->pix_fmt, cocec_context->width, cocec_context->height, PIX_FMT_BGR24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
                fir = FALSE;
            }
            if(!((playH264VideoClass*)lpParam)->nHWAcceleration)
            {
                picture->data[0] += picture->linesize[0] * (cocec_context->height - 1);
                picture->linesize[0] *= -1;
                picture->data[1] += picture->linesize[1] * (cocec_context->height / 2 - 1);
                picture->linesize[1] *= -1;
                picture->data[2] += picture->linesize[2] * (cocec_context->height / 2 - 1);
                picture->linesize[2] *= -1;

                sws_scale(pSwsCtx, picture->data, picture->linesize, 0, cocec_context->height, picRGB->data, picRGB->linesize);//效率？

                SetStretchBltMode(m_hdc, COLORONCOLOR);//这种模式依然不行
                RECT rect;
                GetWindowRect(((playH264VideoClass*)lpParam)->paramUser.playHandle, &rect);
                ((playH264VideoClass*)lpParam)->paramUser.playHeight = rect.bottom - rect.top;
                ((playH264VideoClass*)lpParam)->paramUser.playWidth = rect.right - rect.left;

                ((playH264VideoClass*)lpParam)->playBMPbuf(picRGB, cocec_context->width, cocec_context->height, ((playH264VideoClass*)lpParam)->paramUser.playWidth, ((playH264VideoClass*)lpParam)->paramUser.playHeight, m_hdc, hmemDC, (uint8_t *)bmpBuf, hd);
            }
        }
        catch(...)
        {
            continue;
        }
    }
    try
    {
        if(picture->height >= 1080)
        {
            availableGPU[currentGPU] -= 300;
        }
        else if(picture->height >= 960)
        {
            availableGPU[currentGPU] -= 250;
        }
        else if(picture->height >= 720)
        {
            availableGPU[currentGPU] -= 140;
        }
        else
        {
            availableGPU[currentGPU] -= 60;
        }

        //av_free(&picture->data[0]);
        av_frame_free(&picture);
        //av_free(&picRGB->data[0]);

        av_frame_free(&pFrameYUV);
        avcodec_close(cocec_context);

        if(cocec_context->opaque)
        {
            dxva_Delete((dxva_t *)cocec_context->opaque);
        }
        av_freep(cocec_context);
        //avcodec_free_context(&cocec_context);
        delete[] netBuf;
        delete[] bmpBuf;
        ((playH264VideoClass*)lpParam)->dataQueueClean();

    }
    catch(...)
    {
        return 0;
    }

}

void playH264VideoClass::dataQueueClean()
{
    dataNode* p_data_node_temp;

    while(m_DataQueue.try_pop(p_data_node_temp))
    {
        if(NULL != p_data_node_temp->data)
        {
            delete[] p_data_node_temp->data;
        }
        if(NULL != p_data_node_temp)
        {
            delete p_data_node_temp;
        }
    }
}