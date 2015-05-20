#pragma once

#include "Udp.h"
#include "VBuffer.h"
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

//typedef struct 
//{
//	UINT8 head;
//	UINT8 PT;
//	UINT8 length[2];
//	UINT8 SSRC[4];
//	UINT8 NTP[8];
//	UINT8 RTP[4];
//	UINT8 packetCount[4];
//	UINT8 octetCount[4];
//} RtcpSR;
//
//typedef struct 
//{
//	UINT8 head;
//	UINT8 PT;
//	UINT8 length[2];
//	UINT8 SSRC[4];
//	UINT8 SSRC_1[4];//SSRC of first source对应SR的SSRC
//	UINT8 fractionLost;//从上个sr或rr以来的丢包率,表现为接收方此段时间内期待的RTP包与所收到的RTP包数目的差值和他期待的RTP报文的数目的比值，若为负值，置为0
//	UINT8 cumulationLost[3];//累计丢包率
//	UINT8 EHSNR[4];//extended highest sequence number received
//	UINT8 interJitter[4];//到达时间抖动
//	UINT8 LSR[4];//last SR
//	UINT8 DLSR[4];//delay since last SR
//} RtcpRR;
//
//typedef struct 
//{
//	UINT8 head;
//	UINT8 PT;
//	UINT8 length[2];
//	UINT8 SSRC[4];//
//	UINT8 user[100];//用户描述
//}RtcpSDES;//这个按协议为扩张，但这里只取单一数据块情况
//
//typedef struct 
//{
//	RtcpSR SR;
//	RtcpSDES SDES;
//}recieveSRFrom;
//
//typedef struct
//{
//	RtcpRR RR;
//	RtcpSDES SDES;
//} sendRRTo;

class Rtcp : public Udp
{
public:

	Rtcp(UINT mtu = 1500);
	virtual ~Rtcp();

	virtual int Read(BYTE* pBuffer, UINT16 bufferSize,UINT nTimeOut = 0);

	virtual int Write(UINT nTimeOut = 500000);

	virtual int Handle(BYTE* pBuffer, UINT16 bufferSize);

	void initSdt();

	static int ssrc;

	void ParseMrl(string mrl);

private:
	string IP;
	int port;
protected:

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
};

