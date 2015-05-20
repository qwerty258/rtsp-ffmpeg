// Tcp.cpp: implementation of the libTcp class.
//
//////////////////////////////////////////////////////////////////////

#include "Tcp.h"

Tcp::Tcp()
{
	m_isConnect = FALSE;
}

Tcp::~Tcp()
{
	Close();
}

BOOL Tcp::Open(PCSTR bindIp, INT bindPort)
{
	int error = 0;
	int i_val = 0;
	UINT inIp;

	Close();

	m_Socket= socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if ( m_Socket == INVALID_SOCKET )
	{
		ReportError();
		return FALSE;
	}

	//i_val = 1;	// 阻塞方式
	//error = ioctlsocket(m_Socket, FIONBIO, (ULONG*)&i_val);
	//if (error == SOCKET_ERROR)
	//{
	//	ReportError();
	//	return FALSE;
	//}

	//设置使用keep alive    
	i_val = 1;
	error = setsockopt(m_Socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&i_val, sizeof(i_val));
	if ( error == SOCKET_ERROR )
	{
		ReportError();
		return FALSE;
	}

	tcp_keepalive	inKeepAlive		= {0};
    tcp_keepalive	outKeepAlive	= {0};
    u_long			ulBytesReturn	= 0;
      
	//keep alive为10秒3次
	inKeepAlive.onoff = 1;
	inKeepAlive.keepaliveinterval = 10000;
	inKeepAlive.keepalivetime = 3;
                    
    error = WSAIoctl(m_Socket, SIO_KEEPALIVE_VALS, 
						(LPVOID)&inKeepAlive, sizeof(inKeepAlive), 
                        (LPVOID)&outKeepAlive, sizeof(outKeepAlive), 
						&ulBytesReturn, NULL, NULL);
	if ( error == SOCKET_ERROR )
	{
		ReportError();
		return FALSE;
	}
	
	// 可重用
	i_val = 1;
	error = setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&i_val, sizeof(i_val));
	if (error == SOCKET_ERROR)
	{
		ReportError();
		return FALSE;
	}

	// 绑定套接字
	inIp = inet_addr(bindIp);

	m_BindAddr.sin_family = AF_INET;   
	m_BindAddr.sin_port = htons(bindPort);
	m_BindAddr.sin_addr.s_addr = inIp;
	if (inIp == htonl(INADDR_ANY) || inIp == INADDR_BROADCAST || IN_MULTICAST(ntohl(inIp))  )
		m_BindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	error = bind(m_Socket, (SOCKADDR*)&m_BindAddr, sizeof(m_BindAddr));
	if (error == SOCKET_ERROR)
	{
		ReportError();
		return FALSE;
	}

	m_isOpen = TRUE;
	return TRUE;
}

BOOL Tcp::Connect(PCSTR connectIp, INT connectPort)
{
	if (!m_isOpen)
		return FALSE;

	m_ConnectAddr.sin_family = AF_INET;
	m_ConnectAddr.sin_port = htons(connectPort);
	m_ConnectAddr.sin_addr.s_addr = inet_addr(connectIp);

	if (connect(m_Socket, (SOCKADDR*)&m_ConnectAddr, sizeof(m_ConnectAddr)) == SOCKET_ERROR)
	{
		int isErr = WSAGetLastError();
		printf("Socket error is:%d\n", isErr);
		return FALSE;
	}

	m_isConnect = TRUE;
	return TRUE;
}

void Tcp::Close()
{
	if (!m_Socket)
		return;
	
	memset((PVOID)&m_BindAddr, 0, sizeof(m_BindAddr));
	memset((PVOID)&m_ConnectAddr, 0, sizeof(m_ConnectAddr));

	m_isConnect = FALSE;

	Socket::Close();
}


INT Tcp::ReadLine(string* pStr, UINT strMaxSize, UINT nTimeOut)
{
	char c;
	int iRead = 0;

	if (!m_isConnect || !pStr)
		return -1;
	
	pStr->erase(pStr->begin(), pStr->end());

	while(iRead != -1)
	{
		iRead = Socket::Read((PBYTE)&c, 1, nTimeOut);
		if (iRead <= 0)
			continue;
		if ( c == '\r' || c == '\n')
			break;
		if (iRead == 1  && pStr->size() <= strMaxSize)
			pStr->append(1, c);
	}
	
	if ( iRead <= 0 )
		return -1;

	return (UINT)pStr->size();
}
