#ifndef __SOCKET_H__
#define __SOCKET_H__

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#pragma comment(lib,"Ws2_32.lib")

#include <cerrno>
#include <vector>
using namespace std;

// select mode 
#define SELECT_MODE_READY   0x001
#define SELECT_MODE_WRITE	0x002

// select return codes 
#define SELECT_STATE_READY         0
#define SELECT_STATE_ERROR         1
#define SELECT_STATE_ABORTED       2
#define SELECT_STATE_TIMEOUT       3

class Socket
{
public:
    Socket(UINT mtu = 1500);
    virtual ~Socket();

    virtual void Close();

    virtual int Write(PBYTE pBuffer, int writeSize, UINT nTimeOut = 500000);  // 0.5sec
    virtual int Read(BYTE* pBuffer, int readSize, UINT nTimeOut = 1500000); // 1.5sec

    virtual SOCKADDR_IN GetBindAddr();
    virtual SOCKADDR_IN GetConnectAddr();

    virtual	UINT GetMTU();

    static BOOL GetLocalIPList(vector<string>& vIPList);
    static BOOL GetAdapterSpeed(vector<int>& vList);

protected:
    void	ReportError();
    int		Select(int mode, int timeoutUsec);

    BOOL	m_isOpen;

    SOCKET		m_Socket;
    SOCKADDR_IN m_BindAddr;
    SOCKADDR_IN m_ConnectAddr;

    UINT	m_Mtu;
};

#endif //__SOCKET_H__