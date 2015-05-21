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

    unsigned char *data = new unsigned char[bufsize];
    memcpy(data, buf, bufsize);
    dataNode * m_dataNode = new dataNode;
    m_dataNode->size = bufsize;
    m_dataNode->data = data;

    m_DataQueue.push(m_dataNode);

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
    try
    {
        dataNode *m_dataNode;
        int size;
        if(m_DataQueue.try_pop(m_dataNode))
        {
            if(m_dataNode->size == STOPVIDEO)
            {
                delete m_dataNode;
                return STOPVIDEO;
            }
            memcpy(buf, m_dataNode->data, m_dataNode->size);
            size = m_dataNode->size;

            delete m_dataNode->data;
            delete m_dataNode;

            return size;
        }
        return -1;


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
    int height, width;
    //启用编码器 
    AVPacket  avp;
    playH264VideoClass *VideoClass = (playH264VideoClass *)lpParam;

    AVCodec *codec;
    AVCodecContext *c = NULL;
    int bufsize, len;
    int got_picture;
    AVFrame *picture, *picRGB;
    AVFrame *pFrameYUV = NULL;

    // av_register_all();
    int PictureSize;
    uint8_t *buf = NULL;
    char *netBuf = new char[PICMAX];
    char *bmpBuf = new char[1920 * 1080 * 3];

    if((netBuf == NULL) || (bmpBuf = NULL)) return -1;

    HWND hd = VideoClass->paramUser.playHandle;

    HDC m_hdc = ::GetDC(hd);
    HDC hmemDC = CreateCompatibleDC(m_hdc);
    if(m_hdc == NULL) return 0;

    //c=avcodec_alloc_context3(codec);
    picture = avcodec_alloc_frame();
    picRGB = avcodec_alloc_frame();

    pFrameYUV = avcodec_alloc_frame();
    AVCodecID codeType;
    if(VideoClass->type == 2)
        codeType = CODEC_ID_MPEG4;
    if(VideoClass->type == 1)
        codeType = CODEC_ID_H264;
    if(VideoClass->type == 3)
        codeType = CODEC_ID_FLV1;


    codec = avcodec_find_decoder(codeType);
    c = avcodec_alloc_context3(codec);

    if(VideoClass->nHWAcceleration)
        mAVCodecContextInit(c);

    if(avcodec_open2(c, codec, NULL) < 0)
    {
        return 0;
    }

    extern int availableGPU[8];
    extern int currentGPU;

    struct SwsContext *pSwsCtx = NULL;

    for(;;)
    {
        bufsize = VideoClass->getNextNetBuf(netBuf, PICMAX);

        if(bufsize == STOPVIDEO)
        {
            Sleep(1);
            break;
        }
        if(bufsize <= 0)
        {

            Sleep(1);
            continue;
        }

        av_init_packet(&avp);
        avp.data = (uint8_t *)netBuf;
        avp.size = bufsize;

        //h264输出
        int nWH = 0;
        if(VideoClass->H264Func&&nWH)
        {
            VideoClass->H264Func(VideoClass->INSTANCE, (char *)avp.data, avp.size, picture->width, picture->height);
            continue;
        }

        try
        {
            if(fir&&VideoClass->nHWAcceleration)
            {
                while(bGPlayWnd)
                    Sleep(1);
                bGPlayWnd = 1;
                gPlayWnd = hd;

            }


            len = avcodec_decode_video2(c, picture, &got_picture, &avp);

            if(len < 0)
            {
                //Sleep(1);
                break;
            }

            if(fir&&VideoClass->nHWAcceleration)
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

                if(VideoClass->yuvFunc)
                {
                    pFrameYUV->width = picture->width;
                    pFrameYUV->height = picture->height;
                    int numBytes = avpicture_get_size(PIX_FMT_YUV420P, picture->width, picture->height);
                    avpicture_fill((AVPicture *)pFrameYUV, (uint8_t *)av_malloc(numBytes*sizeof(uint8_t)), PIX_FMT_YUV420P, picture->width, picture->height);
                }
                bGPlayWnd = 0;//打开开关
                fir = false;
            }
            if(!c->opaque&&VideoClass->nHWAcceleration)//p_va失败
            {

                Sleep(1);
                continue;
            }
            //h264输出
            if(VideoClass->H264Func)
            {
                VideoClass->H264Func(VideoClass->INSTANCE, (char *)avp.data, avp.size, picture->width, picture->height);
                nWH = 1;
                continue;
            }
            if(VideoClass->nHWAcceleration)
                DxPictureCopy(c, picture, pFrameYUV, VideoClass->yuvFunc);//内部代码改为直接显示



            if(VideoClass->yuvFunc != NULL&&VideoClass->nHWAcceleration)
            {
                unsigned char * data = new unsigned char[2000 * 1500 / 4 * 6];
                for(int i = 0; i < c->height; i++)
                {
                    memcpy(data + i*c->width, pFrameYUV->data[0] + i*pFrameYUV->linesize[0], c->width);
                }
                for(int i = 0; i < c->height / 2; i++)
                {
                    memcpy(data + c->width*c->height + i*c->width / 2, pFrameYUV->data[2] + i*pFrameYUV->linesize[2], c->width / 2);
                }
                for(int i = 0; i < c->height / 2; i++)
                {
                    memcpy(data + c->width*c->height / 4 * 5 + i*c->width / 2, pFrameYUV->data[1] + i*pFrameYUV->linesize[1], c->width / 2);
                }
                VideoClass->yuvFunc(data, c->height, c->width, VideoClass->userBuffer);
                delete data;
            }
            if(VideoClass->yuvFunc != NULL&&!VideoClass->nHWAcceleration)
            {
                unsigned char * data = new unsigned char[2000 * 1500 / 4 * 6];
                for(int i = 0; i < c->height; i++)
                {
                    memcpy(data + i*c->width, picture->data[0] + i*picture->linesize[0],/*c->width*/1280);
                }
                for(int i = 0; i < c->height / 2; i++)
                {
                    memcpy(data + c->width*c->height + i*c->width / 2, picture->data[2] + i*picture->linesize[2], 640/*c->width/2*/);
                }
                for(int i = 0; i < c->height / 2; i++)
                {
                    memcpy(data + c->width*c->height / 4 * 5 + i*c->width / 2, picture->data[1] + i*picture->linesize[1], 640/*c->width/2*/);
                }
                VideoClass->yuvFunc(data, c->width, c->height, VideoClass->userBuffer);
                delete data;
            }

            if(fir&&!VideoClass->nHWAcceleration)
            {
                width = c->width;
                height = c->height;
                PictureSize = avpicture_get_size(PIX_FMT_BGR24, c->width, c->height);
                buf = (uint8_t*)av_malloc(PictureSize);
                if(buf == NULL)
                {
                    break;
                }
                VideoClass->bmpinfo.biWidth = c->width;
                VideoClass->bmpinfo.biHeight = c->height;
                avpicture_fill((AVPicture *)picRGB, buf, PIX_FMT_BGR24, c->width, c->height);
            }

            if((width != c->width) || (height != c->height) && !VideoClass->nHWAcceleration)
            {
                av_free(buf);
                width = c->width;
                height = c->height;
                PictureSize = avpicture_get_size(PIX_FMT_BGR24, c->width, c->height);
                buf = (uint8_t*)av_malloc(PictureSize);
                if(buf == NULL)
                {
                    break;
                }
                VideoClass->bmpinfo.biWidth = c->width;
                VideoClass->bmpinfo.biHeight = c->height;
                avpicture_fill((AVPicture *)picRGB, buf, PIX_FMT_BGR24, c->width, c->height);
            }

            if(!VideoClass->nHWAcceleration&&fir)
            {
                pSwsCtx = sws_getContext(c->width, c->height, c->pix_fmt, c->width, c->height, PIX_FMT_BGR24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
                fir = FALSE;
            }
            if(!VideoClass->nHWAcceleration)
            {
                picture->data[0] += picture->linesize[0] * (c->height - 1);
                picture->linesize[0] *= -1;
                picture->data[1] += picture->linesize[1] * (c->height / 2 - 1);
                picture->linesize[1] *= -1;
                picture->data[2] += picture->linesize[2] * (c->height / 2 - 1);
                picture->linesize[2] *= -1;

                sws_scale(pSwsCtx, picture->data, picture->linesize, 0, c->height, picRGB->data, picRGB->linesize);//效率？

                SetStretchBltMode(m_hdc, COLORONCOLOR);//这种模式依然不行
                RECT rect;
                GetWindowRect(VideoClass->paramUser.playHandle, &rect);
                VideoClass->paramUser.playHeight = rect.bottom - rect.top;
                VideoClass->paramUser.playWidth = rect.right - rect.left;

                VideoClass->playBMPbuf(picRGB, c->width, c->height, VideoClass->paramUser.playWidth, VideoClass->paramUser.playHeight, m_hdc, hmemDC, (uint8_t *)bmpBuf, hd);
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
          availableGPU[currentGPU] -= 300;

        else if(picture->height >= 960)
          availableGPU[currentGPU] -= 250;

        else if(picture->height >= 720)
          availableGPU[currentGPU] -= 140;

        else
            availableGPU[currentGPU] -= 60;
        //av_free(&picture->data[0]);
        av_frame_free(&picture);
        //av_free(&picRGB->data[0]);

        av_frame_free(&pFrameYUV);
        avcodec_close(c);

        if(c->opaque)
        {
            dxva_Delete((dxva_t *)c->opaque);
        }
        av_freep(c);
        //avcodec_free_context(&c);
        delete[] netBuf;
        //delete[] bmpBuf;
        VideoClass->dataQueueClean();

    }
    catch(...)
    {
        return 0;
    }

}

void playH264VideoClass::dataQueueClean()
{
    dataNode *m_dataNode;

    while(m_DataQueue.try_pop(m_dataNode))
    {
        delete m_dataNode->data;
        delete m_dataNode;
    }
}