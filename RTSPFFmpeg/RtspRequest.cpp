#include "RtspRequest.h"

#include <algorithm>

RtspRequest::RtspRequest()
{
    m_State = stateInit;
}

RtspRequest::~RtspRequest()
{
    Close();
}

BOOL RtspRequest::Open(PCSTR mrl, PCSTR bindIp, INT bindPort)
{
    if(m_State > stateInit)
        return FALSE;

    string preSuffix;
    string suffix;
    INT	   port;

    m_RequestsMrl = mrl;

    if(!Rtsp::ParseMrl(m_RequestsMrl, &preSuffix, &suffix, &port))
        return FALSE;

    if(!Tcp::Open(bindIp, bindPort))
        return FALSE;

    if(!Tcp::Connect(preSuffix.c_str(), port))
        return FALSE;

    //printf("\nConnect server: %s port:%i addr:%s\n\n", preSuffix.c_str(), port, suffix.c_str());

    m_State = stateConnected;

    //if ( !RequestOptions() )
    //	return FALSE;

    return TRUE;
}

BOOL RtspRequest::RequestOptions()
{
    if(m_State < stateConnected)
        return FALSE;

    SendRequest("OPTIONS");

    printf("\n");

    if(!GetResponses())
        return FALSE;

    return TRUE;
}
BOOL RtspRequest::RequestOptions_test(char *name, char *pwd)
{
    if(m_State < stateConnected)
        return FALSE;

    SendRequest_test("OPTIONS", name, pwd);

    printf("\n");

    if(!GetResponses())
        return FALSE;

    return TRUE;
}

BOOL RtspRequest::RequestDescribe(string* pDescribe)
{
    if(m_State < stateConnected)
        return FALSE;

    SendRequest("DESCRIBE");

    printf("\n");

    if(!GetResponses())
        return FALSE;

    if(!GetDescribe(pDescribe))
        return FALSE;

    return TRUE;
}
BOOL RtspRequest::RequestDescribe_test(string* pDescribe, char *name, char *pwd)
{
    if(m_State < stateConnected)
        return FALSE;

    SendRequest_test("DESCRIBE", name, pwd);

    //printf("\n");

    if(!GetResponses())
        return FALSE;

    if(!GetDescribe(pDescribe))
        return FALSE;
#ifdef log
    FILE *fp;
    fp = fopen("C:\\sdp.log","a+");
    fputs(pDescribe->c_str(),fp);
    fputc('\n',fp);
    fclose(fp);
#endif
    Sleep(20);

    return TRUE;
}

BOOL RtspRequest::RequestSetup(PCSTR setupName, INT transportMode, INT clientPort, INT clientRtcpPort, char* pSession)
{
    if(m_State < stateConnected)
        return FALSE;

    string transportField;

    if(setupName == NULL)
        m_SetupName = "";

    if(!GenerateTransportField(&transportField, transportMode, clientPort, clientRtcpPort))
        return FALSE;

    AddField(transportField);
    SendRequest("SETUP");

    printf("\n");

    if(!GetResponses())
        return FALSE;

    m_State = stateReady;

    if(pSession)
        pSession = m_Session;

    return TRUE;
}
BOOL RtspRequest::RequestSetup_test(PCSTR setupName, INT transportMode, INT clientPort, INT clientRtcpPort, char* pSession, char *name, char *pwd)
{
    if(m_State < stateConnected)
        return FALSE;

    string transportField;

    if(setupName == NULL)
        m_SetupName = "";

    if(!GenerateTransportField(&transportField, transportMode, clientPort, clientRtcpPort))
        return FALSE;

    AddField(transportField);
    SendRequest_test("SETUP", name, pwd);

    printf("\n");

    if(!GetResponses())
        return FALSE;

    m_State = stateReady;

    if(pSession)
        pSession = m_Session;

    return TRUE;
}

BOOL RtspRequest::RequestPlay()
{
    if(m_State < stateReady)
        return FALSE;

    SendRequest("PLAY");

    printf("\n");

    if(!GetResponses())
        return FALSE;

    m_State = statePlaying;

    return TRUE;
}

BOOL RtspRequest::RequestPlay_test(char *name, char *pwd)
{
    if(m_State < stateReady)
        return FALSE;

    SendRequest_test("PLAY", name, pwd);

    printf("\n");

    if(!GetResponses())
        return FALSE;

    m_State = statePlaying;

    return TRUE;
}

BOOL RtspRequest::RequestPause()
{
    if(m_State < statePlaying)
        return FALSE;

    SendRequest("PAUSE");

    printf("\n");

    if(!GetResponses())
        return FALSE;

    m_State = statePause;

    return TRUE;
}
BOOL RtspRequest::RequestPause_test(char *name, char *pwd)
{
    if(m_State < statePlaying)
        return FALSE;

    SendRequest_test("PAUSE", name, pwd);

    printf("\n");

    if(!GetResponses())
        return FALSE;

    m_State = statePause;

    return TRUE;
}

BOOL RtspRequest::RequestTeardown()
{
    if(m_State < stateConnected)
        return FALSE;

    SendRequest("TEARDOWN");

    printf("\n");

    //if ( !GetResponses() )
    //	return FALSE;

    m_State = stateInit;

    return TRUE;
}
BOOL RtspRequest::RequestTeardown_test(char *name, char *pwd)
{
    if(m_State < stateConnected)
        return FALSE;

    SendRequest_test("TEARDOWN", name, pwd);

    printf("\n");

    //if ( !GetResponses() )
    //	return FALSE;

    m_State = stateInit;

    return TRUE;
}

void RtspRequest::Close()
{
    Tcp::Close();

    m_State = stateInit;

    printf("\nDisconnection server!\n");
}


BOOL RtspRequest::GetDescribe(string* pDescribe)
{
    BYTE* pDescribeBuffer = NULL;
    int describeSize;
    string describe;
    string searchField;

    if(!SearchResponses(&searchField, "Content-Length"))
        return FALSE;

    describeSize = atoi(searchField.c_str());
    pDescribeBuffer = new BYTE[describeSize + 1];
    if(!pDescribeBuffer)
        return FALSE;
    memset(pDescribeBuffer, 0, describeSize);

    describeSize = Tcp::Read(pDescribeBuffer, describeSize);
    if(describeSize != describeSize)
    {
        delete[]pDescribeBuffer;
        return FALSE;
    }
    pDescribeBuffer[describeSize] = '\0';

    *pDescribe = (char*)pDescribeBuffer;
    //提取字符串供setup使用
    int tip;
    //寻找video码流
    tip = pDescribe->find("m=video", 0);

    if(tip < 0) return false;

    //在video码流中寻找解码格�?
    int deTip = pDescribe->find("a=rtpmap:96 MP4V-ES", tip + 1);
    if(deTip >= 0)
        Decode = 2;
    else if(deTip < 0)
    {
        int deTip = pDescribe->find("a=rtpmap:96 H264", tip + 1);
        if(deTip >= 0)
            Decode = 1;
        else
            return false;
    }

    //
    //寻找是否有给定帧�?
    frame = -1;
    int frameTip = pDescribe->find("a=framerate:", tip + 1);
    if(frameTip >= 0)
        frame = atoi(pDescribe->c_str() + frameTip + 12);

    //

    int videoTip = pDescribe->find("a=control:", tip + 1);

    if(*(pDescribe->c_str() + videoTip + 10) == 't')
    {
        char videotemp[200];
        int i = 1;
        videotemp[0] = '0';//代表添加
        while(*(pDescribeBuffer + videoTip + 10) != '\r')
        {
            videotemp[i] = *(pDescribeBuffer + videoTip + 10);
            i++;
            videoTip++;
        }
        videotemp[i] = '\0';
        m_SetupName_video = videotemp;
    }
    else
    {
        char videotemp[200];
        int i = 1;
        videotemp[0] = '1';//代表完整
        while(*(pDescribeBuffer + videoTip + 10) != '\r')
        {
            videotemp[i] = *(pDescribeBuffer + videoTip + 10);
            i++;
            videoTip++;
        }
        videotemp[i] = '\0';
        m_SetupName_video = videotemp;
    }
    //寻找audio码流

    tip = pDescribe->find("m=audio", 0);

    if(tip > 0)
    {

        int audioTip = pDescribe->find("a=control:", tip + 1);

        if(*(pDescribe->c_str() + audioTip + 10) == 't')
        {
            char audiotemp[200];
            int i = 1;
            audiotemp[0] = '0';//代表添加
            while(*(pDescribeBuffer + audioTip + 10) != '\r')
            {
                audiotemp[i] = *(pDescribeBuffer + audioTip + 10);
                i++;
                audioTip++;
            }
            audiotemp[i] = '\0';
            m_SetupName_audio = audiotemp;
        }
        else
        {
            char audiotemp[200];
            int i = 1;
            audiotemp[0] = '1';//代表完整
            while(*(pDescribeBuffer + audioTip + 10) != '\r')
            {
                audiotemp[i] = *(pDescribeBuffer + audioTip + 10);
                i++;
                audioTip++;
            }
            audiotemp[i] = '\0';
            m_SetupName_audio = audiotemp;
        }
    }

    delete[]pDescribeBuffer;
    return TRUE;
}

void RtspRequest::SendRequest(string requestType)
{
    string requestCmd;
    char cseq[256];
    char session[256];

    m_CSeq++;

    if(m_SetupName.length())
    {
        requestCmd = requestType;
        requestCmd += " ";

        if(*m_SetupName.c_str() == '0')
        {
            requestCmd += m_RequestsMrl;
            requestCmd += "/";
        }
        requestCmd += m_SetupName.c_str() + 1;
        requestCmd += " RTSP/1.0";

    }
    else
    {
        requestCmd = requestType;
        requestCmd += " ";
        requestCmd += m_RequestsMrl;
        requestCmd += " RTSP/1.0";
    }

    _snprintf(cseq, 256, "CSeq: %u", m_CSeq);

    if(requestType.compare("TEARDOWN") == 0)
        m_Session[0] = 0;
    _snprintf(session, 256, "Session: %s", m_Session);

    Write(requestCmd.c_str());
    Write(cseq);

    if(m_Session[0] > 0)
        Write(session);
    Write("User-Agent: LibVLC/2.1.3 (LIVE555 Streaming Media v2014.01.21)");//infito摄像头必须加�?
    WriteFields();
    //Write("Authorization: Basic YWRtaW46MTIzNDU=");
    Write("");
}
//码表
unsigned char EncodeIndex[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};
unsigned char DecodeIndex[] = {
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x3E, 0x40, 0x40, 0x40, 0x3F,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
    0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x40, 0x40, 0x40, 0x40, 0x40};

//baseh264解码函数
inline char GetCharIndex(char c) //内联函数可以省去函数调用过程，提�? 
{
    if((c >= 'A') && (c <= 'Z'))
    {
        return c - 'A';
    }
    else if((c >= 'a') && (c <= 'z'))
    {
        return c - 'a' + 26;
    }
    else if((c >= '0') && (c <= '9'))
    {
        return c - '0' + 52;
    }
    else if(c == '+')
    {
        return 62;
    }
    else if(c == '/')
    {
        return 63;
    }
    else if(c == '=')
    {
        return 0;
    }
    return 0;
}
int fnBase64Decode(char *lpString, char *lpSrc, int sLen)   //解码函数  
{
    static char lpCode[4];
    register int vLen = 0;
    if(sLen % 4)        //Base64编码长度必定�?的倍数，包�?='  
    {
        lpString[0] = '\0';
        return -1;
    }
    while(sLen > 2)      //不足三个字符，忽�? 
    {
        lpCode[0] = GetCharIndex(lpSrc[0]);
        lpCode[1] = GetCharIndex(lpSrc[1]);
        lpCode[2] = GetCharIndex(lpSrc[2]);
        lpCode[3] = GetCharIndex(lpSrc[3]);

        *lpString++ = (lpCode[0] << 2) | (lpCode[1] >> 4);
        *lpString++ = (lpCode[1] << 4) | (lpCode[2] >> 2);
        *lpString++ = (lpCode[2] << 6) | (lpCode[3]);

        lpSrc += 4;
        sLen -= 4;
        vLen += 3;
    }
    return vLen;
}
//base64加码函数
void Base64_Encode(unsigned char* src, unsigned char* dest, int srclen)
{
    int sign = 0;
    for(int i = 0; i != srclen; i++, src++, dest++)
    {
        switch(sign)
        {
            case 0:
                *(dest) = EncodeIndex[*src >> 2];
                break;
            case 1:
                *dest = EncodeIndex[((*(src - 1) & 0x03) << 4) | (((*src) & 0xF0) >> 4)];
                break;
            case 2:
                *dest = EncodeIndex[((*(src - 1) & 0x0F) << 2) | ((*(src)& 0xC0) >> 6)];
                *(++dest) = EncodeIndex[(*(src)& 0x3F)];
                break;
        }
        (sign == 2) ? (sign = 0) : (sign++);
    }
    switch(sign)
    {
        case 0:
            break;
        case 1:
            *(dest++) = EncodeIndex[((*(src - 1) & 0x03) << 4) | (((*src) & 0xF0) >> 4)];
            *(dest++) = '=';
            *(dest++) = '=';
            break;
        case 2:
            *(dest++) = EncodeIndex[((*(src - 1) & 0x0F) << 2) | ((*(src)& 0xC0) >> 6)];
            *(dest++) = '=';
            break;
        default:
            break;
    }
}

void RtspRequest::SendRequest_test(string requestType, char *name, char *pwd)
{
    string requestCmd;
    char cseq[256];
    char session[256];

    m_CSeq++;

    if(m_SetupName.length())
    {
        requestCmd = requestType;
        requestCmd += " ";

        if(*m_SetupName.c_str() == '0')
        {
            requestCmd += m_RequestsMrl;
            requestCmd += "/";
        }
        requestCmd += m_SetupName.c_str() + 1;
        requestCmd += " RTSP/1.0";
    }
    else
    {
        requestCmd = requestType;
        requestCmd += " ";
        requestCmd += m_RequestsMrl;
        requestCmd += " RTSP/1.0";
    }

    _snprintf(cseq, 256, "CSeq: %u", m_CSeq);

    if(requestType.compare("TEARDOWN") == 0)
        m_Session[0] = 0;
    _snprintf(session, 256, "Session: %s", m_Session);

    Write(requestCmd.c_str());
    Write(cseq);

    Write("User-Agent: LibVLC/2.1.3 (LIVE555 Streaming Media v2014.01.21)");

    if(m_Session[0] > 0)
        Write(session);
    //写入授权
    char tempAuth[200];
    strcpy(tempAuth, name);
    strcat(tempAuth, ":");
    strcat(tempAuth, pwd);
    int i;
    for(i = 0; tempAuth[i] != '\0'; i++);
    char auth[280];
    Base64_Encode((unsigned char *)tempAuth, (unsigned char *)auth, i);
    for(i = 0; auth[i] > 0; i++);
    char *auth1 = new char[i + 1];
    memcpy(auth1, auth, i);
    auth[i] = '\0';
    char authfinal[200] = "Authorization: Basic ";
    memcpy(authfinal + 21, auth1, i);
    Write(authfinal);
    delete auth1;
    WriteFields();
    Write("");
}
BOOL RtspRequest::GetResponses()
{
    string str;
    string::size_type iFind;
    int cseq;
    //INT64 session;
    char session[256];
    int iRead = 1;

    m_Responses.clear();

    while(iRead > 0)
    {
        iRead = Read(str);

        //中心力维的相机会立马发送rtcp包，在这份代码里要去除这个干�?只有�?4 ** 00 **的模式时才有效，不然要重新设计代�?
        /*if(*((char *)str.c_str()) == 0x24)
        {
        int size;
        Socket::Read((BYTE *)&size,1);
        char leave[0xFF];
        Socket::Read((BYTE*)leave,size);
        continue;
        }*/

        if(iRead > 0)
        {
            iFind = str.find("CSeq:");
            if(iFind != -1)
            {
                cseq = atoi(str.substr(iFind + 5).c_str());
                if(m_CSeq != cseq)
                {
                    //printf("warning: CSeq mismatch. got %u, assumed %u\n", cseq, m_CSeq);
                    m_CSeq = cseq;
                }
            }

            iFind = str.find("Session:");
            if(iFind != -1)
            {
                //session = _atoi64( str.substr(iFind+8).c_str() );//字符数字混搭就不行了
                int head, tail;
                head = iFind + 8;
                tail = iFind + 8;
                while(*(str.c_str() + head) == ' ')
                    head++;
                while(*(str.c_str() + tail) != ';'&&tail <= str.length() - 1)
                    tail++;

                strcpy(session, str.substr(head, tail - head).c_str());
                if(m_Session[0] == 0)
                {
                    strcpy(m_Session, session);
                    //printf("setting session id to: %I64u\n", m_Session);
                }
            }
            //FILE *fp;
            //fp = fopen("c:\\123.log","a+");
            //fputs(str.c_str(),fp);
            //fputc('\n',fp);
            //fclose(fp);
            m_Responses.push_back(str);
        }
    }

    if(!m_Responses.size())
        return FALSE;

    if(m_Responses[0].find("RTSP/1.0 200 OK") == -1)
        return FALSE;

    return TRUE;
}

BOOL RtspRequest::SearchResponses(string* pStr, string field)
{
    UINT iResponse;
    string::size_type iFind;
    string search;

    transform(field.begin(), field.end(), field.begin(), tolower);

    for(iResponse = 0; iResponse < m_Responses.size(); iResponse++)
    {
        search = m_Responses[iResponse];
        transform(search.begin(), search.end(), search.begin(), tolower);

        iFind = search.find(field);
        if(iFind != string::npos)
        {
            *pStr = m_Responses[iResponse];

            //去除头部多余字符
            pStr->erase(0, iFind + field.size());

            iFind = pStr->find_first_not_of(':');
            if(iFind >= 0)
                pStr->erase(0, iFind);

            iFind = pStr->find_first_not_of(' ');
            if(iFind >= 0)
                pStr->erase(0, iFind);

            return TRUE;
        }
    }
    return FALSE;
}

BOOL RtspRequest::GenerateTransportField(string *pTransport, int streamingMode, int clientRtpPort, int clientRtcpPort)
{
    char temp[10];

    if(!pTransport)
        return FALSE;

    *pTransport = "Transport: ";

    switch(streamingMode)
    {
        case transportModeRtpUdp:
            *pTransport += ("RTP/AVP;");
            break;
        case transportModeRtpTcp:
            *pTransport += "RTP/AVP/TCP;";
            break;
        case transportModeRawUdp:
            *pTransport += "RAW/RAW/UDP;";
            break;
        default:
            return FALSE;
    }

    *pTransport += "unicast;";
    *pTransport += "interleaved = ";

    _snprintf(temp, 10, "%u", clientRtpPort);
    *pTransport += temp;

    if(clientRtcpPort)
    {
        _snprintf(temp, 10, "%u", clientRtcpPort);

        *pTransport += "-";
        *pTransport += temp;
    }
    return TRUE;
}