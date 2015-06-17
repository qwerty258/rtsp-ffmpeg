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
    bool first_round = TRUE; // whether first in loop
    int height = 0;
    int width = 0;
    // enable encoder
    AVPacket avp;
    AVCodec* p_AVCodec = NULL;
    AVCodecContext* p_AVCodecContext = NULL;

    dataNode* p_data_node_temp = NULL;
    int got_picture;

    // for decode
    AVFrame* p_AVFrame_for_decode = av_frame_alloc();
    // for pixel format conversion begin
    AVFrame* p_AVFrame_for_RGB = av_frame_alloc();
    AVFrame* p_AVFrame_for_YUV420 = av_frame_alloc();

    SwsContext* p_SwsContext_for_RGB = NULL;
    // for pixel format conversion end

    int PictureSize;
    uint8_t *buf = NULL;

    HWND hd = static_cast<CDecode*>(lpParam)->paramUser.playHandle;

    HDC hDC = GetDC(hd);
    HDC hMemoryDC = CreateCompatibleDC(hDC);
    if(NULL == hMemoryDC)
    {
        return 0;
    }

    AVCodecID codeType;

    switch(static_cast<CDecode*>(lpParam)->type)
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

    p_AVCodec = avcodec_find_decoder(codeType);
    p_AVCodecContext = avcodec_alloc_context3(p_AVCodec);

    if(static_cast<CDecode*>(lpParam)->m_b_hardware_acceleration)
    {
        mAVCodecContextInit(p_AVCodecContext);
    }

    if(0 > avcodec_open2(p_AVCodecContext, p_AVCodec, NULL))
    {
        return 0;
    }

    extern int availableGPU[8];
    extern int currentGPU;

    for(;;)
    {
        p_data_node_temp = static_cast<CDecode*>(lpParam)->getNextNetBuf();

        if(NULL == p_data_node_temp)
        {
            Sleep(30);
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
        if(static_cast<CDecode*>(lpParam)->m_trace_lost_package)
        {
            FILE* pFile = fopen("C:\\frame.log", "ab");
            char temp[256];
            sprintf(temp, "frame ID: %08X, lost: %08X\n", p_data_node_temp->frame_ID, p_data_node_temp->number_of_lost_frame);
            fwrite(temp, 1, strlen(temp), pFile);
            fclose(pFile);
        }
#endif

        int nWH = 0;
        // h264 callback output, give out data directly
        if(NULL != static_cast<CDecode*>(lpParam)->m_p_function_H264)
        {
            static_cast<CDecode*>(lpParam)->m_p_function_H264(
                static_cast<CDecode*>(lpParam)->m_decode_instance,
                (char*)p_data_node_temp->data,
                p_data_node_temp->size,
                p_AVFrame_for_decode->width,
                p_AVFrame_for_decode->height,
                static_cast<CDecode*>(lpParam)->m_p_H264_extra_data,
                p_data_node_temp->number_of_lost_frame);
        }

        // if YUV420 YV12 and RGB dispaly not set, there is no need to decode anything
        if(NULL == static_cast<CDecode*>(lpParam)->m_p_function_YUV420 &&
           NULL == static_cast<CDecode*>(lpParam)->m_p_function_YV12 &&
           NULL == static_cast<CDecode*>(lpParam)->paramUser.playHandle)
        {
            continue;
        }

        av_init_packet(&avp);
        avp.data = (uint8_t*)p_data_node_temp->data;
        avp.size = p_data_node_temp->size;

        if(first_round && static_cast<CDecode*>(lpParam)->m_b_hardware_acceleration)
        {
            while(bGPlayWnd)
                Sleep(1);
            bGPlayWnd = 1;
            gPlayWnd = hd;
        }


        if(0 > avcodec_decode_video2(p_AVCodecContext, p_AVFrame_for_decode, &got_picture, &avp))
        {
            break;
        }

        // count GPU usage
        if(first_round && static_cast<CDecode*>(lpParam)->m_b_hardware_acceleration)
        {
            //GT620 total number: 1000
            // 1080, GPU consume: 300
            //  960, GPU consume: 250
            //  720, GPU consume: 140
            //   D1, GPU consume: 60

            if(p_AVFrame_for_decode->height >= 1080)
            {
                availableGPU[currentGPU] += 300;
            }
            else if(p_AVFrame_for_decode->height >= 960)
            {
                availableGPU[currentGPU] += 250;
            }
            else if(p_AVFrame_for_decode->height >= 720)
            {
                availableGPU[currentGPU] += 140;
            }
            else
            {
                availableGPU[currentGPU] += 60;
            }
            if(NULL != static_cast<CDecode*>(lpParam)->m_p_function_YUV420)
            {
                p_AVFrame_for_YUV420->width = p_AVFrame_for_decode->width;
                p_AVFrame_for_YUV420->height = p_AVFrame_for_decode->height;
                avpicture_fill(
                    (AVPicture*)p_AVFrame_for_YUV420,
                    (uint8_t*)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, p_AVFrame_for_decode->width, p_AVFrame_for_decode->height)),
                    AV_PIX_FMT_YUV420P,
                    p_AVFrame_for_decode->width,
                    p_AVFrame_for_decode->height);
            }
            bGPlayWnd = 0;// open switch
            first_round = false;
        }
        if(NULL == p_AVCodecContext->opaque && static_cast<CDecode*>(lpParam)->m_b_hardware_acceleration)//p_va failure
        {
            continue;
        }

        if(static_cast<CDecode*>(lpParam)->m_b_hardware_acceleration)
        {
            DxPictureCopy(
                p_AVCodecContext,
                p_AVFrame_for_decode,
                p_AVFrame_for_YUV420,
                static_cast<CDecode*>(lpParam)->m_p_function_YUV420);// internal code change to display directly
        }



        if(NULL != static_cast<CDecode*>(lpParam)->m_p_function_YUV420 && static_cast<CDecode*>(lpParam)->m_b_hardware_acceleration)
        {
            unsigned char * data = new unsigned char[2000 * 1500 / 4 * 6];
            for(int i = 0; i < p_AVCodecContext->height; i++)
            {
                memcpy(
                    data + i*p_AVCodecContext->width,
                    p_AVFrame_for_YUV420->data[0] + i*p_AVFrame_for_YUV420->linesize[0],
                    p_AVCodecContext->width);
            }
            for(int i = 0; i < p_AVCodecContext->height / 2; i++)
            {
                memcpy(
                    data + p_AVCodecContext->width*p_AVCodecContext->height + i*p_AVCodecContext->width / 2,
                    p_AVFrame_for_YUV420->data[2] + i*p_AVFrame_for_YUV420->linesize[2],
                    p_AVCodecContext->width / 2);
            }
            for(int i = 0; i < p_AVCodecContext->height / 2; i++)
            {
                memcpy(
                    data + p_AVCodecContext->width*p_AVCodecContext->height / 4 * 5 + i*p_AVCodecContext->width / 2,
                    p_AVFrame_for_YUV420->data[1] + i*p_AVFrame_for_YUV420->linesize[1],
                    p_AVCodecContext->width / 2);
            }
            static_cast<CDecode*>(lpParam)->m_p_function_YUV420(
                static_cast<CDecode*>(lpParam)->m_decode_instance,
                (char*)data,
                avpicture_get_size(
                    PIX_FMT_YUV420P,
                    p_AVCodecContext->width,
                    p_AVCodecContext->height),
                p_AVCodecContext->height,
                p_AVCodecContext->width,
                p_data_node_temp->frame_ID,
                static_cast<CDecode*>(lpParam)->m_p_YUV420_extra_data,
                p_data_node_temp->number_of_lost_frame);
            delete[] data;
        }
        if(NULL != static_cast<CDecode*>(lpParam)->m_p_function_YUV420 && !static_cast<CDecode*>(lpParam)->m_b_hardware_acceleration)
        {
            unsigned char * data = new unsigned char[2000 * 1500 / 4 * 6];
            for(int i = 0; i < p_AVCodecContext->height; i++)
            {
                memcpy(
                    data + i*p_AVCodecContext->width,
                    p_AVFrame_for_decode->data[0] + i*p_AVFrame_for_decode->linesize[0],
                    /*cocec_context->width*/1280);
            }
            for(int i = 0; i < p_AVCodecContext->height / 2; i++)
            {
                memcpy(
                    data + p_AVCodecContext->width*p_AVCodecContext->height + i*p_AVCodecContext->width / 2,
                    p_AVFrame_for_decode->data[2] + i*p_AVFrame_for_decode->linesize[2],
                    640/*cocec_context->width/2*/);
            }
            for(int i = 0; i < p_AVCodecContext->height / 2; i++)
            {
                memcpy(
                    data + p_AVCodecContext->width*p_AVCodecContext->height / 4 * 5 + i*p_AVCodecContext->width / 2,
                    p_AVFrame_for_decode->data[1] + i*p_AVFrame_for_decode->linesize[1],
                    640/*cocec_context->width/2*/);
            }

            static_cast<CDecode*>(lpParam)->m_p_function_YUV420(
                static_cast<CDecode*>(lpParam)->m_decode_instance,
                (char*)data,
                avpicture_get_size(
                    PIX_FMT_YUV420P,
                    p_AVCodecContext->width,
                    p_AVCodecContext->height),
                p_AVCodecContext->width,
                p_AVCodecContext->height,
                p_data_node_temp->frame_ID,
                static_cast<CDecode*>(lpParam)->m_p_YUV420_extra_data,
                p_data_node_temp->number_of_lost_frame);

            delete[] data;
        }

        if(first_round&&!static_cast<CDecode*>(lpParam)->m_b_hardware_acceleration)
        {
            width = p_AVCodecContext->width;
            height = p_AVCodecContext->height;
            PictureSize = avpicture_get_size(AV_PIX_FMT_BGR24, p_AVCodecContext->width, p_AVCodecContext->height);
            buf = (uint8_t*)av_malloc(PictureSize);
            if(buf == NULL)
            {
                break;
            }
            static_cast<CDecode*>(lpParam)->bmpinfo.biWidth = p_AVCodecContext->width;
            static_cast<CDecode*>(lpParam)->bmpinfo.biHeight = p_AVCodecContext->height;
            avpicture_fill((AVPicture*)p_AVFrame_for_RGB, buf, AV_PIX_FMT_BGR24, p_AVCodecContext->width, p_AVCodecContext->height);
        }

        if((width != p_AVCodecContext->width) || (height != p_AVCodecContext->height) && !static_cast<CDecode*>(lpParam)->m_b_hardware_acceleration)
        {
            av_free(buf);
            width = p_AVCodecContext->width;
            height = p_AVCodecContext->height;
            PictureSize = avpicture_get_size(AV_PIX_FMT_BGR24, p_AVCodecContext->width, p_AVCodecContext->height);
            buf = (uint8_t*)av_malloc(PictureSize);
            if(buf == NULL)
            {
                break;
            }
            static_cast<CDecode*>(lpParam)->bmpinfo.biWidth = p_AVCodecContext->width;
            static_cast<CDecode*>(lpParam)->bmpinfo.biHeight = p_AVCodecContext->height;
            avpicture_fill((AVPicture*)p_AVFrame_for_RGB, buf, AV_PIX_FMT_BGR24, p_AVCodecContext->width, p_AVCodecContext->height);
        }

        if(!static_cast<CDecode*>(lpParam)->m_b_hardware_acceleration&&first_round)
        {
            p_SwsContext_for_RGB = sws_getContext(p_AVCodecContext->width, p_AVCodecContext->height, p_AVCodecContext->pix_fmt, p_AVCodecContext->width, p_AVCodecContext->height, AV_PIX_FMT_BGR24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
            first_round = FALSE;
        }
        if(!static_cast<CDecode*>(lpParam)->m_b_hardware_acceleration)
        {
            p_AVFrame_for_decode->data[0] += p_AVFrame_for_decode->linesize[0] * (p_AVCodecContext->height - 1);
            p_AVFrame_for_decode->linesize[0] *= -1;
            p_AVFrame_for_decode->data[1] += p_AVFrame_for_decode->linesize[1] * (p_AVCodecContext->height / 2 - 1);
            p_AVFrame_for_decode->linesize[1] *= -1;
            p_AVFrame_for_decode->data[2] += p_AVFrame_for_decode->linesize[2] * (p_AVCodecContext->height / 2 - 1);
            p_AVFrame_for_decode->linesize[2] *= -1;

            sws_scale(
                p_SwsContext_for_RGB,
                p_AVFrame_for_decode->data,
                p_AVFrame_for_decode->linesize,
                0,
                p_AVCodecContext->height,
                p_AVFrame_for_RGB->data,
                p_AVFrame_for_RGB->linesize);// efficient ???

            SetStretchBltMode(hDC, COLORONCOLOR);// this pattern still does not work
            RECT rect;
            GetWindowRect(static_cast<CDecode*>(lpParam)->paramUser.playHandle, &rect);
            static_cast<CDecode*>(lpParam)->paramUser.playHeight = rect.bottom - rect.top;
            static_cast<CDecode*>(lpParam)->paramUser.playWidth = rect.right - rect.left;

            static_cast<CDecode*>(lpParam)->playBMPbuf(
                p_AVFrame_for_RGB,
                p_AVCodecContext->width,
                p_AVCodecContext->height,
                static_cast<CDecode*>(lpParam)->paramUser.playWidth,
                static_cast<CDecode*>(lpParam)->paramUser.playHeight,
                hDC,
                hMemoryDC,
                (uint8_t*)static_cast<CDecode*>(lpParam)->m_BMP_buffer,
                hd);
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

    if(p_AVFrame_for_decode->height >= 1080)
    {
        availableGPU[currentGPU] -= 300;
    }
    else if(p_AVFrame_for_decode->height >= 960)
    {
        availableGPU[currentGPU] -= 250;
    }
    else if(p_AVFrame_for_decode->height >= 720)
    {
        availableGPU[currentGPU] -= 140;
    }
    else
    {
        availableGPU[currentGPU] -= 60;
    }

    av_frame_free(&p_AVFrame_for_decode);
    av_frame_free(&p_AVFrame_for_YUV420);
    av_frame_free(&p_AVFrame_for_RGB);

    if(NULL != p_SwsContext_for_RGB)
    {
        sws_freeContext(p_SwsContext_for_RGB);
    }

    avcodec_close(p_AVCodecContext);


    if(p_AVCodecContext->opaque)
    {
        dxva_Delete((dxva_t*)p_AVCodecContext->opaque);
    }

    avcodec_free_context(&p_AVCodecContext);

    return 0;
}

CDecode::CDecode()
{
    m_BMP_buffer = new char[1920 * 1080 * 3 + 1024];
    if(NULL == m_BMP_buffer)
    {
        MessageBox(NULL, L"memory new error", NULL, MB_OK);
    }

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

    // FFmpeg pointers begin
    m_p_AVCodec = NULL;
    m_p_AVCodecContext = NULL;
    m_p_AVCodecParserContext = NULL;
    // FFmpeg pointers end

    // for trace lost package begin
    m_previous_sequence_number = 0;
    m_previous_timestamp = 0;
    m_previous_frame_ID = 0;
    m_previous_number_of_lost_package = 0;
    m_frame_ID = 0;
    m_number_of_lost_package = 0;
    m_trace_lost_package = false;
    // for trace lost package end

}

CDecode::~CDecode()
{
    if(NULL != m_BMP_buffer)
    {
        delete[] m_BMP_buffer;
    }
}

int CDecode::playBMPbuf(AVFrame *pFrameRGB, int width, int height, int playW, int playH, HDC playHD, HDC hMemoryDC, uint8_t *BMPbuf, HWND hWnd)
{
    bufptr = BMPbuf;

    hbit = CreateDIBitmap(playHD, &bmpinfo, CBM_INIT, pFrameRGB->data[0], (BITMAPINFO *)&bmpinfo, DIB_RGB_COLORS);
    OldBitmap = (HBITMAP)SelectObject(hMemoryDC, hbit);

    ::StretchBlt(playHD, 0, 0, playW, playH, hMemoryDC, 0, 0, width, height, SRCCOPY);
    SelectObject(hMemoryDC, OldBitmap);
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
        return -1;
    }
    memset(p_data_node_temp, 0x0, sizeof(dataNode));

    p_data_node_temp->data = new unsigned char[bufsize];
    if(NULL == p_data_node_temp->data)
    {
        MessageBox(NULL, L"new memory error", NULL, MB_OK);
        return -1;
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
        hThreadDecode = CreateThread(NULL, 0, videoDecodeQueue, this, 0, &m_decode_thread_ID);

#ifdef _DEBUG // thread log
        FILE* pFile = fopen("C:\\thread.log", "ab");
        char temp[1024];
        sprintf(temp, "%p Created\n", hThreadDecode);
        fwrite(temp, 1, strlen(temp), pFile);
        fclose(pFile);
#endif // thread log end

        return m_decode_thread_ID;
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