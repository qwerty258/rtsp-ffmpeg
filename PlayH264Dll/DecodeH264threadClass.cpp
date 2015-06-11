#include "stdafx.h"
#include "DecodeH264threadClass.h"
#include <atlimage.h>
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

#define PICMAX (500000) // the maximum data buffer size per frame

// global variables about GPU
bool init = false;
LPDIRECTDRAW lpDD = NULL; // pointer to DirectDraw object
DDSURFACEDESC ddsd;       // DirectDraw surface description
LPDIRECTDRAWSURFACE lpPrimary = NULL; //primary pointer to DirectDraw surface

// hardware acceleration module
extern enum PixelFormat DxGetFormat(AVCodecContext *avctx, const enum PixelFormat *pi_fmt);
extern int DxGetFrameBuf(struct AVCodecContext *avctx, AVFrame *pic);
extern int  DxReGetFrameBuf(struct AVCodecContext *avctx, AVFrame *pic);
extern void DxReleaseFrameBuf(struct AVCodecContext *avctx, AVFrame *pic);
extern int DxPictureCopy(struct AVCodecContext *avctx, AVFrame *src, AVFrame* dst, void *);

// efficient GDI version
HWND gPlayWnd = NULL;
int bGPlayWnd = 0;

int mAVCodecContextInit(AVCodecContext* p_AVCodecContext)
{
    p_AVCodecContext->get_buffer = DxGetFrameBuf;
    p_AVCodecContext->reget_buffer = DxReGetFrameBuf;
    p_AVCodecContext->release_buffer = DxReleaseFrameBuf;
    p_AVCodecContext->opaque = NULL;
    // support for hardware decode
    if(p_AVCodecContext->codec_id == CODEC_ID_MPEG1VIDEO || p_AVCodecContext->codec_id == CODEC_ID_MPEG2VIDEO || p_AVCodecContext->codec_id == CODEC_ID_MPEG4 || p_AVCodecContext->codec_id == CODEC_ID_H264 || p_AVCodecContext->codec_id == CODEC_ID_VC1 || p_AVCodecContext->codec_id == CODEC_ID_WMV3)
    {
        p_AVCodecContext->get_format = DxGetFormat;
    }
    return 0;
}

DWORD WINAPI videoDecodeQueue(LPVOID lpParam)
{
    bool fir = TRUE;
    int height = 0;
    int width = 0;
    // enable encoder
    AVPacket avp;
    AVCodec *codec;
    AVCodecContext *c = NULL;

    int len;
    dataNode* p_data_node_temp;
    int got_picture;
    AVFrame *picture, *picRGB;
    AVFrame *pFrameYUV = NULL;

    int PictureSize;
    uint8_t *buf = NULL;

    HWND hd = ((CDecode*)lpParam)->paramUser.playHandle;

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

    switch(((CDecode*)lpParam)->type)
    {
        case 1:
            codeType = CODEC_ID_H264;
            break;
        case 2:
            codeType = CODEC_ID_MPEG4;
            break;
        case 3:
            codeType = CODEC_ID_FLV1;
            break;
        default:
            break;
    }

    codec = avcodec_find_decoder(codeType);
    c = avcodec_alloc_context3(codec);

    if(((CDecode*)lpParam)->m_b_hardware_acceleration)
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
        p_data_node_temp = ((CDecode*)lpParam)->getNextNetBuf();

        if(NULL == p_data_node_temp)
        {
            Sleep(1000 / (((CDecode*)lpParam)->paramUser.fps) - 10);
            continue;
        }

        if(STOPVIDEO == p_data_node_temp->size)
        {
            if(NULL != p_data_node_temp->data)
            {
                delete[] p_data_node_temp->data;
            }

            if(NULL != p_data_node_temp)
            {
                delete p_data_node_temp;
            }

            break;
        }

#ifdef _DEBUG
        if(((CDecode*)lpParam)->m_trace_lost_package)
        {
            FILE* pFile = fopen("C:\\frame.log", "ab");
            char temp[256];
            sprintf(temp, "frame ID: %08X, lost: %08X\n", p_data_node_temp->frame_ID, p_data_node_temp->number_of_lost_frame);
            fwrite(temp, 1, strlen(temp), pFile);
            fclose(pFile);
        }
#endif

        av_init_packet(&avp);
        avp.data = (uint8_t*)p_data_node_temp->data;
        avp.size = p_data_node_temp->size;

        // h264 output
        int nWH = 0;
        if(NULL != ((CDecode*)lpParam)->m_p_function_H264 && nWH)
        {
            ((CDecode*)lpParam)->m_p_function_H264(
                ((CDecode*)lpParam)->m_decode_instance,
                (char*)avp.data,
                avp.size,
                picture->width,
                picture->height,
                ((CDecode*)lpParam)->m_p_H264_extra_data,
                p_data_node_temp->number_of_lost_frame);

            continue;
        }

        if(fir && ((CDecode*)lpParam)->m_b_hardware_acceleration)
        {
            while(bGPlayWnd)
                Sleep(1);
            bGPlayWnd = 1;
            gPlayWnd = hd;
        }


        len = avcodec_decode_video2(c, picture, &got_picture, &avp);

        if(len < 0)
        {
            break;
        }

        if(fir && ((CDecode*)lpParam)->m_b_hardware_acceleration)
        {
            //GT620 total number: 1000
            // 1080, GPU consume: 300
            //  960, GPU consume: 250
            //  720, GPU consume: 140
            //   D1, GPU consume: 60

            if(picture->height >= 1080)
                availableGPU[currentGPU] += 300;

            else if(picture->height >= 960)
                availableGPU[currentGPU] += 250;

            else if(picture->height >= 720)
                availableGPU[currentGPU] += 140;

            else
                availableGPU[currentGPU] += 60;

            if(NULL != ((CDecode*)lpParam)->m_p_function_YUV420)
            {
                pFrameYUV->width = picture->width;
                pFrameYUV->height = picture->height;
                int numBytes = avpicture_get_size(PIX_FMT_YUV420P, picture->width, picture->height);
                avpicture_fill((AVPicture *)pFrameYUV, (uint8_t *)av_malloc(numBytes), PIX_FMT_YUV420P, picture->width, picture->height);
            }
            bGPlayWnd = 0;// open switch
            fir = false;
        }
        if(!c->opaque && ((CDecode*)lpParam)->m_b_hardware_acceleration)//p_va failure
        {
            continue;
        }
        //h264 output
        if(NULL != ((CDecode*)lpParam)->m_p_function_H264)
        {
            ((CDecode*)lpParam)->m_p_function_H264(
                ((CDecode*)lpParam)->m_decode_instance,
                (char*)avp.data,
                avp.size,
                picture->width,
                picture->height,
                ((CDecode*)lpParam)->m_p_H264_extra_data,
                p_data_node_temp->number_of_lost_frame);

            nWH = 1;
            continue;
        }
        if(((CDecode*)lpParam)->m_b_hardware_acceleration)
            DxPictureCopy(c, picture, pFrameYUV, ((CDecode*)lpParam)->m_p_function_YUV420);// internal code change to display directly



        if(NULL != ((CDecode*)lpParam)->m_p_function_YUV420 && ((CDecode*)lpParam)->m_b_hardware_acceleration)
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
            ((CDecode*)lpParam)->m_p_function_YUV420(
                ((CDecode*)lpParam)->m_decode_instance,
                (char*)data,
                avpicture_get_size(
                    PIX_FMT_YUV420P,
                    c->width,
                    c->height),
                c->height,
                c->width,
                p_data_node_temp->frame_ID,
                ((CDecode*)lpParam)->m_p_YUV420_extra_data,
                p_data_node_temp->number_of_lost_frame);
            delete[] data;
        }
        if(((CDecode*)lpParam)->m_p_function_YUV420 != NULL&&!((CDecode*)lpParam)->m_b_hardware_acceleration)
        {
            unsigned char * data = new unsigned char[2000 * 1500 / 4 * 6];
            for(int i = 0; i < c->height; i++)
            {
                memcpy(data + i*c->width, picture->data[0] + i*picture->linesize[0],/*cocec_context->width*/1280);
            }
            for(int i = 0; i < c->height / 2; i++)
            {
                memcpy(data + c->width*c->height + i*c->width / 2, picture->data[2] + i*picture->linesize[2], 640/*cocec_context->width/2*/);
            }
            for(int i = 0; i < c->height / 2; i++)
            {
                memcpy(data + c->width*c->height / 4 * 5 + i*c->width / 2, picture->data[1] + i*picture->linesize[1], 640/*cocec_context->width/2*/);
            }

            ((CDecode*)lpParam)->m_p_function_YUV420(
                ((CDecode*)lpParam)->m_decode_instance,
                (char*)data,
                avpicture_get_size(
                    PIX_FMT_YUV420P,
                   c->width,
                   c->height),
                c->width,
                c->height,
                p_data_node_temp->frame_ID,
                ((CDecode*)lpParam)->m_p_YUV420_extra_data,
                p_data_node_temp->number_of_lost_frame);

            delete[] data;
        }

        if(fir&&!((CDecode*)lpParam)->m_b_hardware_acceleration)
        {
            width = c->width;
            height = c->height;
            PictureSize = avpicture_get_size(PIX_FMT_BGR24, c->width, c->height);
            buf = (uint8_t*)av_malloc(PictureSize);
            if(buf == NULL)
            {
                break;
            }
            ((CDecode*)lpParam)->bmpinfo.biWidth = c->width;
            ((CDecode*)lpParam)->bmpinfo.biHeight = c->height;
            avpicture_fill((AVPicture *)picRGB, buf, PIX_FMT_BGR24, c->width, c->height);
        }

        if((width != c->width) || (height != c->height) && !((CDecode*)lpParam)->m_b_hardware_acceleration)
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
            ((CDecode*)lpParam)->bmpinfo.biWidth = c->width;
            ((CDecode*)lpParam)->bmpinfo.biHeight = c->height;
            avpicture_fill((AVPicture *)picRGB, buf, PIX_FMT_BGR24, c->width, c->height);
        }

        if(!((CDecode*)lpParam)->m_b_hardware_acceleration&&fir)
        {
            pSwsCtx = sws_getContext(c->width, c->height, c->pix_fmt, c->width, c->height, PIX_FMT_BGR24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
            fir = FALSE;
        }
        if(!((CDecode*)lpParam)->m_b_hardware_acceleration)
        {
            picture->data[0] += picture->linesize[0] * (c->height - 1);
            picture->linesize[0] *= -1;
            picture->data[1] += picture->linesize[1] * (c->height / 2 - 1);
            picture->linesize[1] *= -1;
            picture->data[2] += picture->linesize[2] * (c->height / 2 - 1);
            picture->linesize[2] *= -1;

            sws_scale(pSwsCtx, picture->data, picture->linesize, 0, c->height, picRGB->data, picRGB->linesize);// efficient ???

            SetStretchBltMode(m_hdc, COLORONCOLOR);// this pattern still does not work
            RECT rect;
            GetWindowRect(((CDecode*)lpParam)->paramUser.playHandle, &rect);
            ((CDecode*)lpParam)->paramUser.playHeight = rect.bottom - rect.top;
            ((CDecode*)lpParam)->paramUser.playWidth = rect.right - rect.left;

            ((CDecode*)lpParam)->playBMPbuf(picRGB, c->width, c->height, ((CDecode*)lpParam)->paramUser.playWidth, ((CDecode*)lpParam)->paramUser.playHeight, m_hdc, hmemDC, (uint8_t*)((CDecode*)lpParam)->m_BMP_buffer, hd);
        }

        if(NULL != p_data_node_temp->data)
        {
            delete[] p_data_node_temp->data;
        }
        if(NULL != p_data_node_temp)
        {
            delete p_data_node_temp;
        }
    }

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

    av_frame_free(&picture);
    av_frame_free(&pFrameYUV);
    avcodec_close(c);


    if(c->opaque)
    {
        dxva_Delete((dxva_t *)c->opaque);
    }

    avcodec_free_context(&c);

    ((CDecode*)lpParam)->dataQueueClean();

    return 0;
}

CDecode::CDecode()
{
    m_BMP_buffer = new char[1920 * 1080 * 3 + 1024];
    if(NULL == m_BMP_buffer)
    {
        MessageBox(NULL, L"memory new error", NULL, MB_OK);
    }

    readNetBufIndex = 0;
    writeNetBufIndex = 0;
    bpp = 24; //24 colors

    m_b_hardware_acceleration = false;

    // function pointer for callback begin
    m_p_function_YUV420 = NULL;
    m_p_YUV420_extra_data = NULL;
    m_p_function_YV12 = NULL;
    m_p_YV12_extra_data = NULL;
    m_p_function_H264 = NULL;
    m_p_H264_extra_data = NULL;
    // function pointer for callback end

    // initial FFmpeg pointers
    m_p_AVCodec = NULL;
    m_p_AVCodecContext = NULL;
    m_p_AVCodecParserContext = NULL;

    m_frame_ID = 0;
}

CDecode::~CDecode()
{
    if(NULL != m_BMP_buffer)
    {
        delete[] m_BMP_buffer;
    }
}

int CDecode::playBMPbuf(AVFrame *pFrameRGB, int width, int height, int playW, int playH, HDC playHD, HDC hmemDC, uint8_t *BMPbuf, HWND hWnd)
{
    bufptr = BMPbuf;
    //if(fillbmp != NULL)
    //{
    //    fillbmp((char*)bufptr, width, height);
    //}
    //if(func != NULL&&paramUser.isDecode)
    //{
    //    memcpy(bufptr, &bmpheader, sizeof(bmpheader));
    //    memcpy(bufptr + sizeof(bmpheader), &bmpinfo, sizeof(bmpinfo));
    //    memcpy(bufptr + sizeof(bmpheader) + sizeof(bmpinfo), pFrameRGB->data[0], width*height*bpp / 8);
    //    func(m_decode_instance, width, height, (char*)bufptr, sizeof(bmpheader) + sizeof(bmpinfo) + width*height*bpp / 8, BUFBMP);
    //}

    hbit = CreateDIBitmap(playHD, (BITMAPINFOHEADER *)&bmpinfo, CBM_INIT, pFrameRGB->data[0], (BITMAPINFO *)&bmpinfo, DIB_RGB_COLORS);
    OldBitmap = (HBITMAP)SelectObject(hmemDC, hbit);
    //if(funcD != NULL)
    //{
    //    funcD(m_decode_instance, hmemDC);
    //}
    //if(bmpFunc != NULL)
    //{
    //    bmpFunc((char *)pFrameRGB->data[0], width*height * 3, width, height, 0, 0, 0, hWnd);
    //}
    ::StretchBlt(playHD, 0, 0, playW, playH, hmemDC, 0, 0, width, height, SRCCOPY);
    SelectObject(hmemDC, OldBitmap);
    ::DeleteObject(hbit);
    ::DeleteObject(OldBitmap);
    return 0;
}


//////////////////////////////////////////////////////////////////
//function: write buf into netbuf link list, waiting for decode
///////////////////////////////////////////////////////////////////
int CDecode::writeNetBuf(int num, unsigned char *buf, int bufsize)
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
    p_data_node_temp->frame_ID = m_previous_frame_ID;
    p_data_node_temp->number_of_lost_frame = m_previous_number_of_lost_package;

    m_DataQueue.push(p_data_node_temp);

    // get memory usage and decide whether to throw away data

    GlobalMemoryStatusEx(&m_memory_statex);

    m_total_phys_memory = m_memory_statex.ullTotalPhys;
    m_available_phys_memory = m_memory_statex.ullAvailPhys;
    if(m_available_phys_memory / m_total_phys_memory < 0.1)
    {
        if(m_DataQueue.try_pop(p_data_node_temp))
        {
            delete[] p_data_node_temp->data;
            delete p_data_node_temp;
            p_data_node_temp = NULL;
        }
    }

    return 0;
}

int CDecode::setReadPosize(int index, int readsize)
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
dataNode* CDecode::getNextNetBuf(void)
{
    dataNode* p_data_node_temp;
    if(m_DataQueue.try_pop(p_data_node_temp))
    {
        return p_data_node_temp;
    }
    else
    {
        return NULL;
    }
}

int CDecode::InputParam(myparamInput *p1)
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

int CDecode::freeParam(void)
{
    paramUser.stopPlay = STOPVIDEO;
    dataNode * m_dataNode = new dataNode;
    m_dataNode->size = STOPVIDEO;
    m_dataNode->data = NULL;
    m_DataQueue.push(m_dataNode);
    return 0;
}

int CDecode::playVideo()
{
    paramUser.stopPlay = 0;
    return 0;
}

int CDecode::pauseVideo()
{
    paramUser.stopPlay = 0;
    return 0;
}
int CDecode::playResize(int newWidth, int newHeight)
{
    paramUser.playWidth = newWidth;
    paramUser.playHeight = newHeight;
    return 0;
}

void CDecode::dataQueueClean()
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