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
#pragma comment(lib,"avdevice.lib")
#pragma comment(lib,"avfilter.lib")
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"swresample.lib")
#pragma comment(lib,"swscale.lib")


#define PLAYVIDEO 0
#define STOPVIDEO -2
#define PAUSEVIDEO -1
#define ErrorNoMemory -2
#define BUFH264 1
#define BUFBMP 2
typedef struct SwsContext SwScaleContext;    //just for convenience
const int ListCount = 100;

//static SDL_Surface *screen;

typedef int (WINAPI *PFCALLBACK)(int INSTANCE, int width, int height, char *buf, int bufsize, int buftype);//define my CallBack Func

/*半透明填充多边形回调函数（bmp图片Buffer，bmp图片wid，bmp图片high）*/

typedef int(WINAPI *TDrawRectCallBack)(char*, int, int);
typedef int (WINAPI *TDrawLineCallBack)(int, HDC);//define my CallBack Func

/*TBmpCallBack参数说明：char*（BMPbufer）,int(buferSize),bmpWidth,bmpHight,int(帧号),int(通道号),int(数据类型),HWND,句柄*/

typedef int (WINAPI *TBmpCallBack)(char*, int, int, int, int, int, int, HWND);

/*TYUVCallBack参数说明: yuvbuffer流，图片长和宽*/

typedef int (WINAPI *TYUVCallBack)(unsigned char *buffer, int width, int length, void *);

typedef int (WINAPI *TH264CallBack)(int, char *, int len, int wid, int height);
//收到网络数据块
typedef struct
{
    int  fileSize;
    int  FrameNum;
    int  readPos;
    char picBuffer[1024 * 300];
}netBuf, *LPnetBuf;

typedef struct
{
    int width;             //实际宽度
    int height;            //实际高度
    int playWidth;         //显示宽度
    int playHeight;        //显示高度
    int fps;               //播放频率
    HWND playHandle;       //播放句柄
    int stopPlay;          //播放控制
    int playChannle;
    bool isDecode;        //是否解码
    //int  VideoCode;       //码流类型  0，H264  1 mpeg4 
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

    void  dataQueueClean();
private:
    //LPnetBuf   BuffList[ListCount];
    Concurrency::concurrent_queue<dataNode *> m_DataQueue;
    MEMORYSTATUSEX m_memory_statex;

    double m_total_phys_memory;
    double m_available_phys_memory;


    unsigned long int readNetBufIndex;
    unsigned long int writeNetBufIndex;
    unsigned long int wcount;
    int readNetBufIndexPosize;
    HANDLE hThreadDecode, writewait;
    DWORD threadID;
    BITMAPFILEHEADER bmpheader;

    int bpp;
    HBITMAP  hbit;
    HBITMAP  OldBitmap;
    uint8_t *bufptr;


public:
    int writeNetBuf(int num, unsigned char *buf, int bufsize);
    //int getNetBuf(AVCodecContext* m_pCodecContext,AVCodecParserContext * m_parser,char *buf,int bufsize);
    int setReadPosize(int index, int readsize);
    dataNode* getNextNetBuf(void);
    int playResize(int newWidth, int newHeight);
    //int SaveAsBMPbuf(AVFrame *pFrameRGB, int width, int height,uint8_t *BMPbuf);
    int playBMPbuf(AVFrame *pFrameRGB, int width, int height, int playW, int playH, HDC playHD, HDC hmemDC, uint8_t *BMPbuf, HWND);
public:
    myparamInput paramUser;
    int InputParam(myparamInput *p1);
    int freeParam(void);
    int playVideo();
    int pauseVideo();
    PFCALLBACK func;
    TDrawLineCallBack funcD;
    TBmpCallBack bmpFunc;
    TDrawRectCallBack fillbmp;
    TYUVCallBack yuvFunc;
    TH264CallBack H264Func;

    BITMAPINFOHEADER bmpinfo;
    int INSTANCE;
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
    int type;//code type: 1,h264,2,mpeg,3,FLV
    void *userBuffer;
    bool nHWAcceleration;
};

DWORD WINAPI videoDecodeQueue(LPVOID lpParam);
//int SaveAsBMPbuf(AVFrame *pFrameRGB, int width, int height,uint8_t *BMPbuf);
//void DrawImage(HDC hdc,HBITMAP hbmp,COLORREF color,int x,int y,int width,int height,int SrcX,int SrcY);



