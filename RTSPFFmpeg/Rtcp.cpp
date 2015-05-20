#include "stdafx.h"
#include "Rtcp.h"

int Rtcp::ssrc = 0xfa15cb45;//起始值随便定死一个源，以后+1

Rtcp::Rtcp(UINT mtu):Udp(mtu)
{
	memset(&rcvf,0,sizeof(recieveSRFrom));
	memset(&sdt,0,sizeof(sendRRTo));
	initS = 0;//确定第一个包是否被填
	memset(sSeNum,0,2);//最初的包数
	memset(lSeNum,0,2);//上一次发送RR的包数
	memset(eSeNum,0,2);//最后一次的包数
	allGet = 0;//总接受数
	perGet = 0;//单次接受数
	memset(LSR,0,4);//:从reportee端最后收到的Sender Report中NTP timestamp的中32bits.(无则为0) 
	R_S = 0;
	jitter = 0;
}

Rtcp::~Rtcp()
{

}

void Rtcp::ParseMrl(string mrl)
{
	string::size_type iFind, iFindEnd;

	iFind = mrl.find("rtsp://");
	if (iFind == string::npos)
	{
		//printf("rtsp: bad url: %s\n", mrl);
		return;
	}
	mrl.erase(0, iFind + 7);

	iFind = mrl.find(':');
	if (iFind != string::npos)
	{
		iFindEnd = mrl.find('/', iFind);
		if (iFindEnd != string::npos)
		{
			port = atoi( mrl.substr(iFind + 1, iFindEnd - iFind).c_str() );
			mrl.erase(iFind, iFindEnd - iFind);
		}
	}

	iFind = mrl.find('/');
	if (iFind != string::npos)
	{
		IP = mrl.substr(0, iFind - 0);
		mrl.erase(0, iFind + 1);
	}
	else
	{
		IP = mrl;
	}

	port = 554;

}

void Rtcp::initSdt()
{
	sdt.RR.head = 0x81;
	sdt.RR.PT = 0xc9;
	sdt.RR.length[0] = 0x00;
	sdt.RR.length[1] = 0x07;
	memcpy(sdt.RR.SSRC, &ssrc,4);
	ssrc++;
	memcpy(sdt.RR.SSRC_1,rcvf.SR.SSRC,4);
	sdt.RR.fractionLost = 0;
	memset(sdt.RR.cumulationLost,0xff,3);
	memset(sdt.RR.EHSNR,0,4);
	memset(sdt.RR.interJitter,0,4);
	memset(sdt.RR.LSR,0,4);
	memset(sdt.RR.DLSR,0,4);

	sdt.SDES.head = 0x81;
	sdt.SDES.PT = 0xCA;
	sdt.SDES.length[0] = 0x00;
	sdt.SDES.length[1] = 0x06;
	memcpy(sdt.SDES.SSRC,sdt.RR.SSRC,4);
	sdt.SDES.user[0] = 0x01;
	sdt.SDES.user[1] = 0x11;
	char userMsg[17] = "unuseless ending";
	memcpy(sdt.SDES.user+2,userMsg,17);
	sdt.SDES.user[19] = 0;
}

int Rtcp::Read(BYTE* pBuffer, UINT16 bufferSize,UINT nTimeOut)
{
	int iRead =- 1;
	iRead = Socket::Read(pBuffer, bufferSize, nTimeOut);
	return iRead;
}

int Rtcp::Write(UINT nTimeOut)
{
	
	int selectState = 0;
	int sendSize = 0;

	SOCKADDR_IN sendAddr;
	sendAddr.sin_family = AF_INET;
	sendAddr.sin_port = htons(port);
	sendAddr.sin_addr.s_addr = inet_addr((PCSTR)IP.c_str());

	selectState = Select(SELECT_MODE_WRITE, nTimeOut);
	if (selectState == SELECT_STATE_TIMEOUT)
		return 0;

	if (selectState == SELECT_STATE_READY)
	{
		sendSize = sendto(m_Socket, (char*)&sdt, sizeof(sendRRTo),MSG_DONTROUTE,(SOCKADDR*)&sendAddr,sizeof(sendAddr));
		if (sendSize <= 0)
			return -1;
		return sendSize;
	}	
}

void copy(recieveSRFrom *des, recieveSRFrom *src)
{
	des->SR.head = src->SR.head;
	des->SR.PT = src->SR.PT;
	memcpy(des->SR.length,src->SR.length,2);
	memcpy(des->SR.SSRC,src->SR.SSRC,4);
	memcpy(des->SR.NTP,src->SR.NTP,8);
	memcpy(des->SR.RTP,src->SR.RTP,4);
	memcpy(des->SR.packetCount,src->SR.packetCount,4);
	memcpy(des->SR.octetCount,des->SR.octetCount,4);

	des->SDES.head = src->SDES.head;
	des->SDES.PT = src->SDES.PT;
	memcpy(des->SDES.length,src->SDES.length,2);
	memcpy(des->SDES.SSRC,src->SDES.SSRC,4);
	UINT8 tmp[2];
	memcpy(tmp,&src->SDES.length[1],1);
	memcpy(tmp+1,&src->SDES.length[0],1);
	memcpy(des->SDES.user,src->SDES.user,*((unsigned short int*)tmp)*4);
}

int Rtcp::Handle(BYTE* pBuffer, UINT16 bufferSize)
{
	//记录SR数据
	
	recieveSRFrom tmpSR;
	tmpSR.SR.head = pBuffer[0];
	tmpSR.SR.PT = pBuffer[1];
	memcpy(tmpSR.SR.length,pBuffer+RTCP_LENGTH_START,RTCP_LENGTH_SIZE);
	memcpy(tmpSR.SR.SSRC,pBuffer+RTCP_SSRC_START,RTCP_SSRC_SIZE);
	memcpy(tmpSR.SR.NTP,pBuffer+RTCP_NTPTIME_START,RTCP_NTPTIME_SIZE);
	memcpy(tmpSR.SR.RTP,pBuffer+RTCP_RTPTIME_START,RTCP_RTPTIME_SIZE);
	memcpy(tmpSR.SR.packetCount,pBuffer+RTCP_PACKET_START,RTCP_PACKET_SIZE);
	memcpy(tmpSR.SR.octetCount,pBuffer+RTCP_PLAYLOAD_START,RTCP_PLAYLOAD_SIZE);

	//记录SDES数据
	UINT8 tmp[2] = {0};
	memcpy(tmp,&tmpSR.SR.length[1],1);
	memcpy(tmp+1,&tmpSR.SR.length[0],1);
	int SDESStart = (*(unsigned short int*)tmp)*4+4;

	tmpSR.SDES.head = pBuffer[SDESStart];
	tmpSR.SDES.PT = pBuffer[1+SDESStart];
	memcpy(tmpSR.SDES.length,pBuffer+SDESStart+RTCP_LENGTH_START,RTCP_LENGTH_SIZE);
	memcpy(tmpSR.SDES.SSRC,pBuffer+SDESStart+RTCP_SSRC_START,RTCP_SSRC_SIZE);
	memcpy(tmpSR.SDES.user,pBuffer+8,(*(unsigned short int*)tmp)*4);

	memcpy(LSR,tmpSR.SR.NTP+2,4);

	copy(&rcvf,&tmpSR);

	return 0;
}