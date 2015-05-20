#pragma once 
#include "string_t.h"

#include <string>
#include <vector>
using namespace std;

INT cmd_read()
{
	int c;
	string text;

	while(TRUE)
	{
		c = getchar();
		if ( c == '\n' || c == EOF)
			break;
		text += (CHAR)c;
	}	

	return atoi( text.c_str());
}

UINT cmd_read(string& text)
{
	int i,c;

	i = 0;

	text = "";

	while(TRUE)
	{
		c = getchar();
		if ( c == '\n' || c == EOF)
			break;
		text += (CHAR)c;
		i++;

	}
	return i;
}


BOOL GetLocalIPList(vector<string>& vIPList)
{ 
	WORD wVersionRequested; 
	WSADATA wsaData; 
	int err; 
	
	wVersionRequested = MAKEWORD( 2, 2 ); 
	err = WSAStartup( wVersionRequested, &wsaData ); 
	if ( err != 0 ) 	{
		printf("GetLocalIPList	: WSAStartup failed !"); 
		return FALSE; 
	} 
	
	char szhn[256]; 
	int nStatus = gethostname(szhn, sizeof(szhn)); 
	if (nStatus == SOCKET_ERROR ) 	{ 
		printf("Socket	: Gethostname failed, Error code: %d", WSAGetLastError()); 
		return FALSE; 
	} 
	
	vIPList.clear();
	
	HOSTENT *host = gethostbyname(szhn); 
	if (host != NULL) 	{ 
		for ( int i=0; ; i++ ) 		{ 
			vIPList.push_back( inet_ntoa( *(IN_ADDR*)host->h_addr_list[i] ) ) ; 
			if ( host->h_addr_list[i] + host->h_length >= host->h_name ) 
				break; 
		}
	}
	
	WSACleanup(); 
	
	return TRUE; 
}

void PrintfHelp()
{
	printf( "Syntax: RtspSerivce -bindip <string | integer> -bindport <integer> -videofile <string> -audiofile <string>\n"
		"\n"
		"-bindip <string | integer>  Specify the bind ip address.\n"
		"                                Can be input string or integer.\n"
		"                                If the input is string,\n"
		"                                it will be used to bind the IP address.\n"
		"                                If the input is integer,\n"
		"                                the IP address of the n-th NIC will be bound,\n"
		"                                where n is the value of the input.\n"
		"                                Dafelut use the 1th NIC's ip address.\n"
		"-bindport <integer>         Specify the bind port.\n"
		"                                Dafelut bind port is 554.\n"
		"-videofile <string>         Specify the video file path.\n"
		"                                Dafelut file path is \"oh yeah.h264\".\n"
		"-audiofile <string>         Specify the audio file path.\n"
		"                                Dafelut file path is \"oh yeah.aac\".\n"
		"\n");
}

BOOL ParseArg(int argc, TCHAR** argv, 
			  string& textBindIP, UINT& nBindPort, 
			  string& textVideoFile, string& textAudioFile)
{
	vector<string> vLocalIP;
	
	if ( !GetLocalIPList(vLocalIP) || !vLocalIP.size())
	{
		printf("Get local ip address failed!\n");
		return FALSE;
	}
		
	textBindIP = "127.0.0.1";
	nBindPort = RTSP_PROTOCOL_PORT;
	textVideoFile = "oh yeah.h264";
	textAudioFile = "oh yeah.aac";

	if (vLocalIP.size() > 0)
		textBindIP = vLocalIP[0];
	
	for (int i = 0; i < argc; i++)
	{
		string_t arg = *(argv + i);
				
		if ( arg.compare(_T("-bindip")) == 0 && ( ( i + 1) < argc) )
		{
			string_t ip = *(argv + i + 1);
			
			if ( ip.length() > 6)
			{
				textBindIP = t2a(ip);
			}
			else
			{
				UINT iIP = _ttoi( ip.c_str());
				
				if ( iIP < vLocalIP.size())
					textBindIP = vLocalIP[iIP];
			}
		}
		
		if ( arg.compare(_T("-bindport")) == 0 && ( ( i + 1) < argc))
		{
			string_t port = *(argv + i + 1);
			
			UINT nPort = _ttoi( port.c_str());
			
			if ( nPort <= 65535)
				nBindPort = nPort;
		}

		if ( arg.compare(_T("-videofile")) == 0 && ( ( i + 1) < argc))
		{
			textVideoFile = t2a( *(argv + i + 1) );
		}
		
		if ( arg.compare(_T("-audiofile")) == 0 && ( ( i + 1) < argc))
		{
			textAudioFile = t2a( *(argv + i + 1) );
		}
	}
	return TRUE;
}


