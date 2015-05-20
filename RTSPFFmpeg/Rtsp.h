// Rtsp.h: interface for the Rtsp class.
//
//////////////////////////////////////////////////////////////////////

//为了方便，所以所有的rtp和rtcp通讯全部在rtsp类中完成，代码可读性在后续版本中提高
#pragma once

#define RTCP_PT_START		1
#define RTCP_PT_SIZE		1

#define RTCP_LENGTH_START	2
#define RTCP_LENGTH_SIZE	2

#define RTCP_SSRC_START		4
#define RTCP_SSRC_SIZE		4

#define RTCP_NTPTIME_START  8
#define RTCP_NTPTIME_SIZE   8

#define RTCP_RTPTIME_START	16
#define RTCP_RTPTIME_SIZE	4

#define RTCP_PACKET_START	20
#define RTCP_PACKET_SIZE	4

#define RTCP_PLAYLOAD_START 24
#define RTCP_PLAYLOAD_SIZE  4

#define RTSP_PROTOCOL_PORT 554

#include "Tcp.h"
#include <ctime>

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

typedef enum serverState
{
    stateInit = 0,//0
    stateConnected,//1
    stateReady,//2
    statePlaying,//3
    statePause,//4
    stateRecording//5
};

typedef enum streamingTransportMode
{
    transportModeRtpUdp = 1,
    transportModeRtpTcp,
    transportModeRawUdp
};

static const int MAX_FIELDS_COUNT = 256;

class Rtsp : public Tcp
{
public:
    int Decode;//编码器选择1 h264，2 mpeg
    int ID;//解码器号
    bool nfirst;//判断是否为第一个包
public:
    Rtsp();
    virtual ~Rtsp();

    virtual int  Write(string str);
    virtual int  Read(string& str);

    virtual void AddField(string field);
    virtual void WriteFields();

    virtual BOOL ParseMrl(string mrl, string* pPreSuffix, string* pSuffix, int* pPort);

    virtual int Read_Start(int& type, short int *size);

    virtual int Read_Head();

    virtual int Read_PlayLoad(short int len);

    virtual int Read_Leave(int len);

    virtual int Read_RTCPVideo(int len);

    virtual int Write_RTCPVideo(UINT nTimeout = 10);

    virtual int Handle_RTCPVideo(BYTE * buffer, int buffersize);

    virtual void initSdt();

    virtual int Read_Test();

protected:
    int		m_State;
    int		m_CSeq;
    //INT64	m_Session;
    char	m_Session[50];

    vector<string> m_Fields;

public:
    //以下大端格式
    int initS;//确定第一个包是否被填
    unsigned char sSeNum[2];//最初的包数
    unsigned char lSeNum[2];//上一次发送RR的包数
    unsigned char eSeNum[2];//最后一次的包数
    UINT16 allGet;//总接受数
    UINT8 perGet;//单次接受数
    unsigned char LSR[4];//:从reportee端最后收到的Sender Report中NTP timestamp的中32bits.(无则为0) 
    time_t  lTime;//SSRC_n源的上个SR

    int R_S;//R-S
    int jitter;//时间抖动

    recieveSRFrom rcvf;
    sendRRTo sdt;

    static int ssrc;

};