#include "stdafx.h"
#include "Udp.h"

Udp::Udp(UINT mtu) :Socket(mtu)
{
	m_isConnect = FALSE;
}

Udp::~Udp()
{
}

BOOL Udp::Open(string bindIp, int bindPort)
{
	if (m_isOpen)
		return FALSE;

	m_isOpen = FALSE;
	m_isConnect = FALSE;

	int error = 0;
	int i_val = 0;
	
	if (m_Socket)
		closesocket(m_Socket);
	m_Socket= socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if ( m_Socket == INVALID_SOCKET )
	{
		ReportError();
		return FALSE;
	}

	i_val = 1;	// 非阻塞方式
	error = ioctlsocket(m_Socket, FIONBIO, (ULONG*)&i_val);
	if (error == SOCKET_ERROR)
	{
		ReportError();
		return FALSE;
	}

	i_val = (int)(1024 * 1024 * 1.25);//2M Byte 1000Mbps的network在0.01秒内最高可以接收到1.25MB数据
	error = setsockopt( m_Socket, SOL_SOCKET, SO_RCVBUF, (char*)&i_val, sizeof(i_val) );
	if (error == SOCKET_ERROR)
	{
		ReportError();
		return FALSE;
	}

	error = setsockopt( m_Socket, SOL_SOCKET, SO_SNDBUF, (char*)&i_val, sizeof(i_val) );
	if (error == SOCKET_ERROR)
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

	// 设置ttl
	i_val = 5;
	error = setsockopt(m_Socket,IPPROTO_IP,IP_MULTICAST_TTL,(char*)&i_val, sizeof(i_val));
	if (error == SOCKET_ERROR)
	{
		ReportError();
		return FALSE;
	}
	
	// 绑定套接字
	memset((PVOID)&m_BindAddr, 0, sizeof(m_BindAddr));

	m_BindAddr.sin_family = AF_INET;   
	m_BindAddr.sin_port = htons(bindPort);
	m_BindAddr.sin_addr.s_addr = inet_addr(bindIp.c_str());
	
	if ( IN_MULTICAST(ntohl(m_BindAddr.sin_addr.s_addr))  || m_BindAddr.sin_addr.s_addr == INADDR_BROADCAST )
		m_BindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	error = bind(m_Socket, (SOCKADDR*)&m_BindAddr, sizeof(m_BindAddr));
	if (error == SOCKET_ERROR)
	{
		ReportError();
		return FALSE;
	}
	
	i_val = sizeof(m_BindAddr);
	error = getsockname(m_Socket, (SOCKADDR*)&m_BindAddr, &i_val);
	if (error == SOCKET_ERROR)
	{
		ReportError();
		return FALSE;
	}

	if ( ! SetMulticast(bindIp.c_str()))
		return FALSE;

	m_isOpen = TRUE;
	return TRUE;
}

BOOL Udp::Connect(string connectIp, int connectPort)
{
	if (!m_isOpen)
		return FALSE;

	int error = 0;
	int i_val = 0;

	memset((PVOID)&m_ConnectAddr, 0, sizeof(m_ConnectAddr));

	if ( ! SetMulticast(connectIp.c_str()))
		return FALSE;

	m_ConnectAddr.sin_family = AF_INET;
	m_ConnectAddr.sin_port = htons(connectPort);
	m_ConnectAddr.sin_addr.s_addr = inet_addr(connectIp.c_str());

	if (connect(m_Socket, (SOCKADDR*)&m_ConnectAddr, sizeof(m_ConnectAddr)) == SOCKET_ERROR )
	{
		ReportError();
		return FALSE;
	}
	m_isConnect = TRUE;
	return TRUE;
}

int Udp::Read(BYTE* pBuffer, UINT16 bufferSize, UINT nTimeOut)
{
	int iRead;

	if ( !m_isOpen )
		return -1;
	
	iRead = Socket::Read(pBuffer, bufferSize, nTimeOut);

	return iRead;
}

int Udp::Write(PBYTE pBuffer, UINT16 bufferSize, UINT nTimeOut)
{
	int iWrite;

	if ( !m_isOpen || !m_isConnect)
		return -1;

	iWrite = Socket::Write(pBuffer, bufferSize, nTimeOut);

	return iWrite;
}


BOOL Udp::SetMulticast(PCSTR textIP)
{
	int error = 0;
	int i_val = 0;

	// 设置多播和广播
	if ( IN_MULTICAST(ntohl(inet_addr(textIP))) )
	{
		i_val = 1;
		error =  setsockopt(m_Socket, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&i_val, sizeof(i_val) );
		if (error == SOCKET_ERROR)
		{
			ReportError();
			return FALSE;
		}
		
		ip_mreq multicastAddr;
		multicastAddr.imr_multiaddr.s_addr = inet_addr(textIP);
		multicastAddr.imr_interface.s_addr = htonl(INADDR_ANY);
		
		error = setsockopt(m_Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&multicastAddr, sizeof(multicastAddr));
		if (error == SOCKET_ERROR)
		{
			ReportError();
			return FALSE;
		}
	}
	
	//设置广播
	if ( inet_addr(textIP) == INADDR_BROADCAST )
	{
		i_val = 1;
		error = setsockopt( m_Socket, SOL_SOCKET, SO_BROADCAST, (char*)&i_val, sizeof(i_val) );
		if (error == SOCKET_ERROR)
		{
			ReportError();
			return FALSE;
		}
	}
	return TRUE;
}