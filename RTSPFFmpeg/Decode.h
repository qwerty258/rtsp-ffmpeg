#pragma once
#include <Windows.h>
//data struct for decode and play
typedef struct container_myparam_input
{
    int width;             //The actual width
    int height;            //The actual height
    int playWidth;         //Display width
    int playHeight;        //Display height
    int fps;               //frame per second
    HWND playHandle;       //window handle
    int stopPlay;          //play control
    int playChannle;       //play channle
    bool isDecode;
}myparamInput;

typedef int(_cdecl *PFCALLBACK)(int INSTANCE, int width, int height, char *buf, int bufsize, int buftype);
typedef int(_cdecl *TDrawRectCallBack)(char*, int, int);
typedef int(_cdecl *TDrawLineCallBack)(int, HDC);//define my CallBack Func
typedef int(_cdecl *TBmpCallBack)(char*, int, int, int, int, int, int, HWND);
typedef int(_cdecl *TYUVCallBack)(char *, int len, int wid, void *);//YUV callback
typedef int(_cdecl *TH264CallBack)(int, char *, int len, int wid, int height);//H264 callback

typedef int(_cdecl *setYUVCallBack)(int, TYUVCallBack, void *);
typedef int(_cdecl *setH264CallBack)(int, TH264CallBack);
typedef int(_cdecl *fSetCallBack)(int INSTANCE, PFCALLBACK f1);
typedef void(_cdecl *fDecodeVideo)(int num, unsigned char * pInBuffer, int size);
typedef int(_cdecl *finitVideoDLL)(int max_number_of_playing_instance);
typedef int(_cdecl *fGetIdlevideoINSTANCE)();
typedef int(_cdecl *fInitVideoParam)(int INSTANCE, myparamInput *Myparam, int);
typedef int(_cdecl *fInitVideoParamNew)(int, myparamInput *, int, int);
typedef int(_cdecl *fpauseVideos)(int INSTANCE);
typedef int(_cdecl *fplayVideos)(int INSTANCE);
typedef int(_cdecl *ffreeVideos)(int INSTANCE);
typedef int(_cdecl *finputBuf)(int INSTANCE, char *buf, int bufsize);
typedef int(_cdecl *fresize)(int INSTANCE, int width, int height);
typedef void(_cdecl *fexitdll)();
typedef int(_cdecl *fSetDrawLineCallBack)(int INSTANCE, TDrawLineCallBack f1);
typedef int(_cdecl *fSetBmpCallBack)(int INSTANCE, TBmpCallBack bmp1);
typedef int(_cdecl *fSetFillBmpCallBack)(int INSTANCE, TDrawRectCallBack bmpf);
typedef int(_cdecl *revoHW)(int INSTANCE);


typedef int(_cdecl *YUV420Func)(int instance, char* frameBuff, int framebuffsize, int frameHeight, int frameWidth, int frameID, void* userdata, int frameLost); // YUV420 callback


typedef struct
{
    UINT8 head;
    UINT8 PT;
    UINT8 length[2];
    UINT8 SSRC[4];
    UINT8 NTP[8];
    UINT8 RTP[4];
    UINT8 packetCount[4];
    UINT8 octetCount[4];
} RtcpSR;

typedef struct
{
    UINT8 head;
    UINT8 PT;
    UINT8 length[2];
    UINT8 SSRC[4];
    UINT8 SSRC_1[4];//SSRC of first source对应SR的SSRC
    UINT8 fractionLost;//从上个sr或rr以来的丢包率,表现为接收方此段时间内期待的RTP包与所收到的RTP包数目的差值和他期待的RTP报文的数目的比值，若为负值，置为0
    UINT8 cumulationLost[3];//The cumulative lost package
    UINT8 EHSNR[4];//extended highest sequence number received
    UINT8 interJitter[4];//到达时间抖动
    UINT8 LSR[4];//last SR
    UINT8 DLSR[4];//delay since last SR
} RtcpRR;

typedef struct
{
    UINT8 head;
    UINT8 PT;
    UINT8 length[2];
    UINT8 SSRC[4];
    UINT8 user[100]; // User description
}RtcpSDES;
// 这个按协议为扩张，但这里只取单一数据块情�?

typedef struct
{
    RtcpSR SR;
    RtcpSDES SDES;
}recieveSRFrom;

typedef struct
{
    RtcpRR RR;
    RtcpSDES SDES;
} sendRRTo;
