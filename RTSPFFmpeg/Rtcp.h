#pragma once

#include "Udp.h"
#include "Decode.h"
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

class Rtcp : public Udp
{
public:

    Rtcp(UINT mtu = 1500);
    virtual ~Rtcp();

    virtual int Read(BYTE* pBuffer, UINT16 bufferSize, UINT nTimeOut = 0);

    virtual int Write(UINT nTimeOut = 500000);

    virtual int Handle(BYTE* pBuffer, UINT16 bufferSize);

    void initSdt();

    static int ssrc;

    void ParseMrl(string mrl);

private:
    string IP;
    int port;
    void copy(recieveSRFrom *des, recieveSRFrom *src);
protected:

public:
    //以下大端格式
    int initS;//确定第一个包是否被填
    unsigned char sSeNum[2];//最初的包数
    unsigned char lSeNum[2];//上一次发送RR的包�?
    unsigned char eSeNum[2];//最后一次的包数
    UINT16 allGet;//总接受数
    UINT8 perGet;//单次接受�?
    unsigned char LSR[4];//:从reportee端最后收到的Sender Report中NTP timestamp的中32bits.(无则�?) 
    time_t  lTime;//SSRC_n源的上个SR

    int R_S;//R-S
    int jitter;//时间抖动

    recieveSRFrom rcvf;
    sendRRTo sdt;
};

