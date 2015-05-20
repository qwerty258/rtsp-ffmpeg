#ifndef __RTSP_REQUEST_H__
#define __RTSP_REQUEST_H__

#include "Rtsp.h"

class RtspRequest : public Rtsp
{
public:
    RtspRequest();
    virtual ~RtspRequest();

    virtual BOOL Open(PCSTR mrl, PCSTR bindIp = "", INT bindPort = 0);

    virtual BOOL RequestOptions();
    virtual BOOL RequestOptions_test(char *, char *);
    virtual BOOL RequestDescribe(string* pDescribe);
    virtual BOOL RequestDescribe_test(string *, char *, char *);
    virtual BOOL RequestSetup(PCSTR setupName, INT transportMode, INT clientPort, INT clientRtcpPort, char* pSession);
    virtual BOOL RequestSetup_test(PCSTR setupName, INT transportMode, INT clientPort, INT clientRtcpPort, char* pSession, char *, char *);
    virtual BOOL RequestPlay();
    virtual BOOL RequestPlay_test(char *, char *);
    virtual BOOL RequestPause();
    virtual BOOL RequestPause_test(char *, char *);
    virtual BOOL RequestTeardown();
    virtual BOOL RequestTeardown_test(char *, char *);
    //为了看的清楚，以上皆不用重载
    virtual void Close();

    string  m_SetupName;
    string  m_SetupName_audio;
    string	m_SetupName_video;

protected:
    BOOL GetDescribe(string* pDescribe);
    BOOL GetResponses();
    BOOL SearchResponses(string* pStr, string field);

    void SendRequest(string requestType);
    void SendRequest_test(string requestType, char *, char *);

    BOOL ParseTransportField(string transport, int* pStreamingMode, int* pClientRtpPort, int* pClientRtcpPort);
    BOOL GenerateTransportField(string *pTransport, int streamingMode, int clientRtpPort, int clientRtcpPort);

    string  m_RequestsMrl;
    string  m_AuUserAPSD;//存用户名密码，暂时不写
    vector<string> m_Responses;

public:

    int frame;//帧率
};

#endif //__RTSP_REQUEST_H__