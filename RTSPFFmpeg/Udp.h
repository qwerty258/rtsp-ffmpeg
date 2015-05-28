#ifndef __UDP_H__
#define __UDP_H__

#include "Socket.h"
#include <WS2tcpip.h>

#include <string>
using namespace std;

class Udp : public CSocket
{
public:

    Udp(UINT mtu = 1500);
    virtual ~Udp();

    virtual BOOL Open(string bindIp = "", int bindPort = 0);

    virtual BOOL Connect(string connectIp, int connectPort);

    virtual int  Read(BYTE* pBuffer, UINT16 bufferSize, UINT nTimeOut = 500000);

    virtual int  Write(PBYTE pBuffer, UINT16 bufferSize, UINT nTimeOut = 500000);
    //virtual BOOL Close();
protected:

    BOOL SetMulticast(PCSTR textIP);

    BOOL	m_isConnect;
};

#endif //__UDP_H__