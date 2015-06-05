#pragma once
#include <winsock2.h>
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

#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"swscale.lib")


#define PLAYVIDEO 0
#define STOPVIDEO -2
#define PAUSEVIDEO -1
#define ErrorNoMemory -2
#define BUFH264 1
#define BUFBMP 2
typedef struct SwsContext SwScaleContext;    //just for convenience
const int ListCount = 100;

typedef int(*function_YUV420)(int instance, char* frame_buffer, int frame_buffer_size, int frame_width, int frame_height, int frame_ID, void* userdata, int frame_lost); // YUV420 callback
typedef int(*function_YV12)(int instance, char* frame_buff, int frame_buffer_size, int frame_width, int frame_height, void* userdata, int frame_lost); // YV12 callback
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
    int width;             // actual width
    int height;            // actual hight
    int playWidth;         // display width
    int playHeight;        // display hight
    int fps;               // frame per second
    HWND playHandle;       // handle to the paly window
    int stopPlay;          // play control
    int playChannle;
    bool isDecode;        // is decode
    //int  VideoCode;       //code type: 0: H264, 1: mpeg4
}myparamInput;

typedef struct
{
    int size;
    unsigned char * data;
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
    int playResize(int newWidth, int newHeight);
    //int SaveAsBMPbuf(AVFrame *pFrameRGB, int width, int height,uint8_t *BMPbuf);
    int playBMPbuf(AVFrame *pFrameRGB, int width, int height, int playW, int playH, HDC playHD, HDC hmemDC, uint8_t *BMPbuf, HWND);
    void  dataQueueClean();

private:
    //LPnetBuf   BuffList[ListCount];
    Concurrency::concurrent_queue<dataNode*> m_DataQueue;
    MEMORYSTATUSEX m_memory_statex;

    double m_total_phys_memory;
    double m_available_phys_memory;


    unsigned long int readNetBufIndex;
    unsigned long int writeNetBufIndex;
    unsigned long int wcount;
    int readNetBufIndexPosize;
    DWORD threadID;
    BITMAPFILEHEADER bmpheader;

    int bpp;
    HBITMAP  hbit;
    HBITMAP  OldBitmap;
    uint8_t *bufptr;

public:
    myparamInput paramUser;
    int InputParam(myparamInput *p1);
    int freeParam(void);
    int playVideo();
    int pauseVideo();

    HANDLE hThreadDecode, writewait;

    // function pointer for callback begin
    function_YUV420 m_p_function_YUV420;
    void* m_p_YUV420_extra_data;
    function_YV12 m_p_function_YV12;
    void* m_p_YV12_extra_data;
    function_H264 m_p_function_H264;
    void* m_p_H264_extra_data;
    // function pointer for callback end

    BITMAPINFOHEADER bmpinfo;
    void abc()
    {
        writeNetBufIndex++;
    }
    unsigned long int getW()
    {
        return writeNetBufIndex;
    }
    unsigned long int getR()
    {
        return readNetBufIndex;
    }
public:
    int type;//code type: 1: h264, 2: mpeg, 3: FLV
    bool nHWAcceleration;

    char* m_BMP_buffer;
    int m_decode_instance;

    // for FFmpeg
    AVCodecContext*       m_p_AVCodecContext;
    AVFrame*              m_p_AVFrame;
    AVCodecParserContext* m_p_AVCodecParserContext;
    AVCodec*              m_p_AVCodec;
};

//int SaveAsBMPbuf(AVFrame *pFrameRGB, int width, int height,uint8_t *BMPbuf);
//void DrawImage(HDC hdc,HBITMAP hbmp,COLORREF color,int x,int y,int width,int height,int SrcX,int SrcY);
DWORD WINAPI videoDecodeQueue(LPVOID lpParam);
