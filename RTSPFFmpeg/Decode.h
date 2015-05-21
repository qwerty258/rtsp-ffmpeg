#pragma once


//解码播放时用到的数据结构
typedef struct
{
    int width;             //实际宽度
    int height;            //实际高度
    int playWidth;         //显示宽度
    int playHeight;        //显示高度
    int fps;               //播放频率
    HWND playHandle;       //播放句柄
    int stopPlay;          //播放控制
    int playChannle;		//播放的通道号
    bool isDecode;
}myparamInput;

typedef int(_cdecl *PFCALLBACK)(int INSTANCE, int width, int height, char *buf, int bufsize, int buftype);
typedef int(_cdecl *TDrawRectCallBack)(char*, int, int);
typedef int(_cdecl *TDrawLineCallBack)(int, HDC);//define my CallBack Func
typedef int(_cdecl *TBmpCallBack)(char*, int, int, int, int, int, int, HWND);
typedef int(_cdecl *TYUVCallBack)(char *, int len, int wid, void *);//YUV回调
typedef int(_cdecl *TH264CallBack)(int, char *, int len, int wid, int height);//H264回调

typedef int(_cdecl *setYUVCallBack)(int, TYUVCallBack, void *);
typedef int(_cdecl *setH264CallBack)(int, TH264CallBack);
typedef int(_cdecl *fSetCallBack)(int INSTANCE, PFCALLBACK f1);
typedef void(_cdecl *fDecodeVideo)(int num, unsigned char * pInBuffer, int size);
typedef int(_cdecl *finitVideoDLL)();
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
    UINT8 cumulationLost[3];//累计丢包率
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
    UINT8 SSRC[4];//
    UINT8 user[100];//用户描述
}RtcpSDES;//这个按协议为扩张，但这里只取单一数据块情况

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
