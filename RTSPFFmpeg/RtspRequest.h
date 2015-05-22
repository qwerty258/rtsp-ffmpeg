#pragma once
#include "Rtsp.h"

class CRTSPRequest : public Rtsp
{
public:
    CRTSPRequest();
    ~CRTSPRequest();

    BOOL Open(char* mrl, PCSTR bindIp = "", INT bindPort = 0);

    BOOL RequestOptions();
    BOOL RequestOptions(char*, char*);
    BOOL RequestDescribe(string* pDescribe);
    BOOL RequestDescribe(string*, char*, char*);
    BOOL RequestSetup(PCSTR setupName, INT transportMode, INT clientPort, INT clientRtcpPort, char* pSession);
    BOOL RequestSetup(PCSTR setupName, INT transportMode, INT clientPort, INT clientRtcpPort, char* pSession, char*, char*);
    BOOL RequestPlay();
    BOOL RequestPlay(char*, char*);
    BOOL RequestPause();
    BOOL RequestPause(char*, char*);
    BOOL RequestTeardown();
    BOOL RequestTeardown(char*, char*);
    //为了看的清楚，以上皆不用重载
    void Close();

    string  m_SetupName;
    string  m_SetupName_audio;
    string	m_SetupName_video;

protected:
    BOOL GetDescribe(string* pDescribe);
    BOOL GetResponses();
    BOOL SearchResponses(string* pStr, string field);

    void SendRequest(string requestType);
    void SendRequest(string requestType, char*, char*);

    BOOL ParseTransportField(string transport, int* pStreamingMode, int* pClientRtpPort, int* pClientRtcpPort);
    BOOL GenerateTransportField(string *pTransport, int streamingMode, int clientRtpPort, int clientRtcpPort);

    string  m_RequestsMrl;
    string  m_AuUserAPSD;//存用户名密码，暂时不�?
    vector<string> m_Responses;

public:

    int frame;//帧率
};