// Tcp.h: interface for the libTcp class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __TCP_H__
#define __TCP_H__

#include "Socket.h"
#include <WS2tcpip.h>

/* Argument structure for SIO_KEEPALIVE_VALS */

struct tcp_keepalive {
    u_long  onoff;
    u_long  keepalivetime;
    u_long  keepaliveinterval;
};

// New WSAIoctl Options

#define SIO_RCVALL            _WSAIOW(IOC_VENDOR,1)
#define SIO_RCVALL_MCAST      _WSAIOW(IOC_VENDOR,2)
#define SIO_RCVALL_IGMPMCAST  _WSAIOW(IOC_VENDOR,3)
#define SIO_KEEPALIVE_VALS    _WSAIOW(IOC_VENDOR,4)
#define SIO_ABSORB_RTRALERT   _WSAIOW(IOC_VENDOR,5)
#define SIO_UCAST_IF          _WSAIOW(IOC_VENDOR,6)
#define SIO_LIMIT_BROADCASTS  _WSAIOW(IOC_VENDOR,7)
#define SIO_INDEX_BIND        _WSAIOW(IOC_VENDOR,8)
#define SIO_INDEX_MCASTIF     _WSAIOW(IOC_VENDOR,9)
#define SIO_INDEX_ADD_MCAST   _WSAIOW(IOC_VENDOR,10)
#define SIO_INDEX_DEL_MCAST   _WSAIOW(IOC_VENDOR,11)

// Values for use with SIO_RCVALL* options
#define RCVALL_OFF             0
#define RCVALL_ON              1
#define RCVALL_SOCKETLEVELONLY 2

class Tcp : public Socket
{
public:
	Tcp();

	virtual ~Tcp();
	
	virtual BOOL Open(PCSTR bindIp = "", INT bindPort = 0);

	virtual BOOL Connect(PCSTR connectIp, INT connectPort);

	virtual INT ReadLine(string* pStr, UINT strMaxSize, UINT nTimeOut = 500000); //0.5 sec

	virtual void Close();

protected:
	BOOL m_isConnect;
};

#endif // __TCP_H__
