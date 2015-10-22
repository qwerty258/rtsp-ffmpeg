#pragma once
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <concurrent_queue.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>
#include <libswscale\swscale.h>

#ifdef __cplusplus
}
#endif

#include "PlayH264DLL.h"

#define PLAYVIDEO 0
#define STOPVIDEO -2
#define PAUSEVIDEO -1
#define ErrorNoMemory -2
#define BUFH264 1
#define BUFBMP 2
typedef struct SwsContext SwScaleContext;    //just for convenience
const int ListCount = 100;

typedef int(*function_YUV420)(int instance, char* frame_buffer, int frame_buffer_size, int frame_width, int frame_height, size_t frame_ID, void* userdata, int frame_lost); // YUV420 callback
typedef int(*function_NV12)(int instance, char* frame_buff, int frame_buffer_size, int frame_width, int frame_height, void* userdata, int frame_lost); // YV12 callback
typedef int(*function_H264)(int instance, char* frame_buff, int frame_buffer_size, int frame_width, int frame_height, void* userdata, int frame_lost); // h264 callback

// received net packages
typedef struct
{
    int  fileSize;
    int  FrameNum;
    int  readPos;
    char picBuffer[1024 * 300];
}netBuf, *LPnetBuf;


typedef struct
{
    int size;
    unsigned char * data;
    size_t frame_ID;
    int number_of_lost_frame;
}dataNode;


class CDecode
{
public:
    CDecode();
    ~CDecode();
    int writeNetBuf(int num, unsigned char *buf, int bufsize);
    //int getNetBuf(AVCodecContext* m_pCodecContext,AVCodecParserContext * m_parser,char *buf,int bufsize);
    int setReadPosize(int index, int readsize);
    dataNode* getNextNetBuf(void);
    int playResize(void);
    //int SaveAsBMPbuf(AVFrame *pFrameRGB, int width, int height,uint8_t *BMPbuf);
    int playBMPbuf(AVFrame* pFrameRGB, int width, int height, int playW, int playH);
    void dataQueueClean();

private:
    Concurrency::concurrent_queue<dataNode*> m_DataQueue;
    MEMORYSTATUSEX m_memory_statex;

    double m_total_phys_memory;
    double m_available_phys_memory;

    DWORD m_decode_thread_ID;

    int bits_per_pixel;
    HBITMAP  hbit;
    HBITMAP  OldBitmap;
    uint8_t *bufptr;

public:
    myparamInput paramUser;
    int InputParam(myparamInput *p1);
    int freeParam(void);
    int playVideo();
    int pauseVideo();

    HANDLE hThreadDecode;

    // function pointer for callback begin
    function_YUV420 m_p_function_YUV420;
    void* m_p_YUV420_extra_data;
    function_NV12 m_p_function_NV12;
    void* m_p_NV12_extra_data;
    function_H264 m_p_function_H264;
    void* m_p_H264_extra_data;
    function_RGB24 m_p_function_RGB24;
    void* m_p_RGB24_extra_data;
    // function pointer for callback end

    BITMAPINFO bmpinfo;

public:
    int type;//code type: 1: h264, 2: mpeg, 3: FLV
    bool m_b_hardware_acceleration;

    int m_decode_instance;

    // for FFmpeg begin
    AVCodecContext*       m_p_AVCodecContext;
    AVFrame*              m_p_AVFrame;
    AVCodecParserContext* m_p_AVCodecParserContext;
    AVCodec*              m_p_AVCodec;
    // for FFmpeg end

    // for trace lost package begin
    unsigned short m_previous_sequence_number;
    unsigned int   m_previous_timestamp;
    size_t m_previous_frame_ID;
    int m_previous_number_of_lost_package;
    size_t m_frame_ID;
    int m_number_of_lost_package;
    bool m_trace_lost_package;
    // for trace lost package end

    // for GDI paly begin
    HDC m_hDC;
    // for GDI paly end
};

//int SaveAsBMPbuf(AVFrame *pFrameRGB, int width, int height,uint8_t *BMPbuf);
//void DrawImage(HDC hdc,HBITMAP hbmp,COLORREF color,int x,int y,int width,int height,int SrcX,int SrcY);
DWORD WINAPI videoDecodeQueue(LPVOID lpParam);
