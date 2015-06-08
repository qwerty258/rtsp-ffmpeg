// Rtsp.cpp: implementation of the libRtsp class.
//
//////////////////////////////////////////////////////////////////////

#include "Rtsp.h"
#include "UNpack.h"

int Rtsp::ssrc = 0xfa15cb45;//起始值随便定死一个源，以�?1

Rtsp::Rtsp()
{
    m_CSeq = 0;
    m_State = 0;
    m_Session[0] = 0;
    //m_CRTSP_paused = false;
    m_p_RTP_package_buffer = new BYTE[1500];
    if(NULL == m_p_RTP_package_buffer)
    {
        MessageBox(NULL, L"memory new error", NULL, MB_OK);
    }
}

Rtsp::~Rtsp()
{
    if(NULL != m_p_RTP_package_buffer)
    {
        delete[] m_p_RTP_package_buffer;
        m_p_RTP_package_buffer = NULL;
    }
}
int Rtsp::Read_Leave(int len)
{

    char leave[1500];
    int rs = CSocket::Read((BYTE *)leave, len);
    if(rs == -1)
        return -1;

    int tmpLen = len - rs;

    int countT = 0;
    while(tmpLen > 0 && countT < 20)
    {
        rs = CSocket::Read((BYTE*)leave + len - tmpLen, tmpLen);
        if(rs == -1)
            return -1;
        tmpLen = tmpLen - rs;

        countT++;
    }

    if(countT == 20)
    return -1;
    /*char input[10];

        FILE *fp;

        fp = fopen("c:\\45.log","a+");
        itoa(senum,input,10);
        fputs(input,fp);
        fputc('\n',fp);

        itoa(rs,input,10);

        fputs(input,fp);
        fputc('\n',fp);
        fclose(fp);
        senum++;*/

    return 0;
}

int Rtsp::Read_Start(int& type, short int* size)
{
    type = 0;

    unsigned char start[4];

    int iRead = CSocket::Read((BYTE *)start, 4);

    if(iRead < -1)
        return -1;

    int tmpLen = 4 - iRead;

    int countT = 0;//连接20�?

    while(tmpLen > 0 && countT < 20)
    {
        iRead = CSocket::Read((BYTE*)start + 4 - tmpLen, tmpLen);
        if(iRead == -1)
            return -1;
        tmpLen = tmpLen - iRead;
        countT++;
    }
    if(countT == 20)
        return -1;

    if(start[0] != 0x24)
        return -1;

    if(start[1] < 0 || start[1]>3)
        return -1;

    type = start[1];
    char len[2];
    len[0] = start[3];
    len[1] = start[2];

    *size = *(short int *)len;


    //if(start[1] == 0)
    //{
    //FILE *fp;
    //fp = fopen("c:\\654.txt","a+");
    //char input[10];
    //itoa(senum,input,10);
    //fputs(input,fp);
    //fputc('\n',fp);
    //itoa(*size,input,10);
    //fputs(input,fp);
    //fputc('\n',fp);
    //fclose(fp);
    //}
    //if(start[1]>0&&start[1]<4)
    //{
    //FILE *fp;
    //fp = fopen("c:\\54.txt","a+");
    //char input[10];
    //itoa(senum,input,10);
    //fputs(input,fp);
    //fputc('\n',fp);
    //itoa(*size,input,10);
    //fputs(input,fp);
    //fputc('\n',fp);
    //fclose(fp);
    //}
    //if(start[1] == 0)
    //{
    //	FILE * fp;
    //	fp = fopen("c:\\对比.txt","ab+");
    //	fwrite(start+2,1,2,fp);
    //	fputc('\0',fp);
    //	fwrite(&size,1,2,fp);
    //	fputc('\0',fp);
    //	fputc('\0',fp);
    //	fclose(fp);
    //}

    return 0;
}
int Rtsp::Read_Head()
{
    char head[12];
    int iRead = CSocket::Read((BYTE *)head, 12);

    // 套接字读取错�?
    if(iRead == -1)
        return -1;

    return 0;
}
int Rtsp::Read_PlayLoad(short int len)
{
    int iRead = CSocket::Read((BYTE*)m_p_RTP_package_buffer, len);
    if(iRead == -1)
        return -1;

    int tmpLen = len - iRead;

    int countT = 0;//连接20�?

    while(tmpLen > 0 && countT < 20)
    {
        iRead = CSocket::Read((BYTE*)m_p_RTP_package_buffer + len - tmpLen, tmpLen);
        if(iRead == -1)
            return -1;
        tmpLen = tmpLen - iRead;
        countT++;
    }

    if(countT == 20)
        return -1;

    //char a[10] = "c:\\包序";
    //itoa(ID,a+7,10);
    //FILE *fp;
    //fp = fopen(a,"ab+");
    //fwrite(buffer+2,1,2,fp);
    //fclose(fp);

    /*FILE *fp;
    fp = fopen("c:\\�?txt","ab+");
    fwrite(buffer+2,1,2,fp);
    fclose(fp);

    char input[10];
    fp = fopen("c:\\456.log","a+");
    itoa(senum,input,10);
    fputs(input,fp);
    fputc('\n',fp);

    itoa(len,input,10);

    fputs(input,fp);
    fputc('\n',fp);
    fclose(fp);
    senum++;*/

    /*FILE * fp;
    fp = fopen("c:\\检�?log","ab+");
    fputs("SIZE:",fp);

    char s[10];

    itoa(len,s,10);

    fputs(s,fp);

    fputs("Start:",fp);
    fwrite(buffer,1,len,fp);
    fputs("End:",fp);
    fclose(fp);*/

    //FILE *fp;
    //fp = fopen("c:\\20150209","a+");
    //char a[10];
    //itoa(Decode,a,10);
    //fputs(a,fp);
    //fclose(fp);

    if(iRead < 0)
        return -1;

    //if(!m_CRTSP_paused)
    //{
    //    if(Decode == 1)
    //        RTP_unpackage(&buffer[0], len, ID, &nfirst);
    //    if(Decode == 2)
    //        RTP_unpackage_mpeg(buffer, len, ID, &nfirst);
    //}

    if(tmpLen == 0)
    {
        if(initS == 0)
        {
            memcpy(sSeNum, m_p_RTP_package_buffer + 2, 2);
            memcpy(lSeNum, m_p_RTP_package_buffer + 2, 2);
            memcpy(eSeNum, m_p_RTP_package_buffer + 2, 2);

            allGet++;
            perGet++;

            DWORD timeTmp;
            *((char *)&timeTmp) = m_p_RTP_package_buffer[7];
            *((char *)&timeTmp + 1) = m_p_RTP_package_buffer[6];
            *((char *)&timeTmp + 2) = m_p_RTP_package_buffer[5];
            *((char *)&timeTmp + 3) = m_p_RTP_package_buffer[4];

            time_t rawtime;
            time(&rawtime);

            R_S = rawtime - timeTmp;

            initS = 1;
        }
        else
        {
            memcpy(eSeNum, m_p_RTP_package_buffer + 2, 2);
            allGet++;
            perGet++;

            DWORD timeTmp;
            *((char *)&timeTmp) = m_p_RTP_package_buffer[7];
            *((char *)&timeTmp + 1) = m_p_RTP_package_buffer[6];
            *((char *)&timeTmp + 2) = m_p_RTP_package_buffer[5];
            *((char *)&timeTmp + 3) = m_p_RTP_package_buffer[4];

            time_t rawtime;
            time(&rawtime);

            int R_STmp = rawtime - timeTmp;

            int D_ij = 0;
            if(R_STmp - R_S > 0)
                D_ij = R_STmp - R_S;
            else
                D_ij = R_S - R_STmp;

            jitter = jitter + (D_ij - jitter) / 16;

        }
    }

    return len - tmpLen;
}

int Rtsp::Read_RTCPVideo(int len)
{
    char buffer[1500];
    int rs = CSocket::Read((BYTE *)buffer, len);
    if(rs == -1)
        return -1;

    int tmpLen = len - rs;

    int countT = 0;
    while(tmpLen > 0 && countT < 20)
    {
        rs = CSocket::Read((BYTE*)buffer + len - tmpLen, tmpLen);
        if(rs == -1)
            return -1;
        tmpLen = tmpLen - rs;
        countT++;
    }

    if(countT == 20)
    return -1;

    if(tmpLen == 0)
    {
        time(&lTime);
        Handle_RTCPVideo((BYTE *)buffer, len);
    }
}

int Rtsp::Write_RTCPVideo(UINT nTimeOut)
{
    int selectState = 0;
    int sendSize = 0;

    selectState = Select(SELECT_MODE_WRITE, nTimeOut);
    if(selectState == SELECT_STATE_TIMEOUT)
        return 0;

    if(selectState == SELECT_STATE_READY)
    {
        char a1 = 0x24;
        char a2 = 0x01;
        char a3[2] = {0x00, 0x8c};

        sendSize = send(m_Socket, &a1, 1, MSG_DONTROUTE);
        sendSize = send(m_Socket, &a2, 1, MSG_DONTROUTE);
        sendSize = send(m_Socket, &a3[0], 1, MSG_DONTROUTE);
        sendSize = send(m_Socket, &a3[1], 1, MSG_DONTROUTE);
        sendSize = send(m_Socket, (char*)&sdt, sizeof(sendRRTo), MSG_DONTROUTE);
        if(sendSize <= 0)
            return -1;

        //a2 = 0x3;

        //sendSize = send(m_Socket, &a1, 1,MSG_DONTROUTE);
        //sendSize = send(m_Socket, &a2, 1,MSG_DONTROUTE);
        //sendSize = send(m_Socket, &a3[0], 1,MSG_DONTROUTE);
        //sendSize = send(m_Socket, &a3[1], 1,MSG_DONTROUTE);
        //sendSize = send(m_Socket, (char*)&sdt, sizeof(sendRRTo),MSG_DONTROUTE);
        //if (sendSize <= 0)
        //	return -1;
        return sendSize;
    }
}

void Rtsp::copy(recieveSRFrom *des, recieveSRFrom *src)
{
    des->SR.head = src->SR.head;
    des->SR.PT = src->SR.PT;
    memcpy(des->SR.length, src->SR.length, 2);
    memcpy(des->SR.SSRC, src->SR.SSRC, 4);
    memcpy(des->SR.NTP, src->SR.NTP, 8);
    memcpy(des->SR.RTP, src->SR.RTP, 4);
    memcpy(des->SR.packetCount, src->SR.packetCount, 4);
    memcpy(des->SR.octetCount, des->SR.octetCount, 4);

    des->SDES.head = src->SDES.head;
    des->SDES.PT = src->SDES.PT;
    memcpy(des->SDES.length, src->SDES.length, 2);
    memcpy(des->SDES.SSRC, src->SDES.SSRC, 4);
    UINT8 tmp[2];
    memcpy(tmp, &src->SDES.length[1], 1);
    memcpy(tmp + 1, &src->SDES.length[0], 1);
    memcpy(des->SDES.user, src->SDES.user, *((unsigned short int*)tmp) * 4);
}

int Rtsp::Handle_RTCPVideo(BYTE *pBuffer, int len)
{
    //记录SR数据

    recieveSRFrom tmpSR;
    tmpSR.SR.head = pBuffer[0];
    tmpSR.SR.PT = pBuffer[1];
    memcpy(tmpSR.SR.length, pBuffer + RTCP_LENGTH_START, RTCP_LENGTH_SIZE);
    memcpy(tmpSR.SR.SSRC, pBuffer + RTCP_SSRC_START, RTCP_SSRC_SIZE);
    memcpy(tmpSR.SR.NTP, pBuffer + RTCP_NTPTIME_START, RTCP_NTPTIME_SIZE);
    memcpy(tmpSR.SR.RTP, pBuffer + RTCP_RTPTIME_START, RTCP_RTPTIME_SIZE);
    memcpy(tmpSR.SR.packetCount, pBuffer + RTCP_PACKET_START, RTCP_PACKET_SIZE);
    memcpy(tmpSR.SR.octetCount, pBuffer + RTCP_PLAYLOAD_START, RTCP_PLAYLOAD_SIZE);

    //记录SDES数据
    UINT8 tmp[2] = {0};
    memcpy(tmp, &tmpSR.SR.length[1], 1);
    memcpy(tmp + 1, &tmpSR.SR.length[0], 1);
    int SDESStart = (*(unsigned short int*)tmp) * 4 + 4;

    tmpSR.SDES.head = pBuffer[SDESStart];
    tmpSR.SDES.PT = pBuffer[1 + SDESStart];
    memcpy(tmpSR.SDES.length, pBuffer + SDESStart + RTCP_LENGTH_START, RTCP_LENGTH_SIZE);
    memcpy(tmpSR.SDES.SSRC, pBuffer + SDESStart + RTCP_SSRC_START, RTCP_SSRC_SIZE);
    memcpy(tmpSR.SDES.user, pBuffer + 8, (*(unsigned short int*)tmp) * 4);

    memcpy(LSR, tmpSR.SR.NTP + 2, 4);

    copy(&rcvf, &tmpSR);

    return 0;
}

int Rtsp::Read(string& str)
{
    char c;
    int iRead;

    str = "";

    do
    {
        iRead = CSocket::Read((BYTE*)&c, 1);
        if(iRead == 1)
        {
            //中心力维的相机会立马发送rtcp包，在这份代码里要去除这个干�?
            if(c == 0x24)
            {
                char size[2];
                char leave[1500];

                CSocket::Read((BYTE *)leave, 1);//丢弃

                CSocket::Read((BYTE *)(size + 1), 1);
                CSocket::Read((BYTE *)size, 1);
                short int i = 0;
                i = *((int *)size);
                CSocket::Read((BYTE*)leave, i);//丢弃
                continue;
            }

            if(c == '\r' || c == '\n')
                break;

            str.append(1, c);
        }
    } while(iRead == 1);

    if(c == '\r')
        iRead = CSocket::Read((BYTE*)&c, 1);

    // 套接字读取错�?
    if(iRead == -1)
        return -1;

    // 空行代表回应结束
    if(str.size() == 0 && (c == '\r' || c == '\n'))
    {
        printf("<< ''\n\n");
        return 0;
    }

    //if (!iRead || !pStr->size())
    //	return -1;
    if(str.size())
        printf("<< '%s'\n", str.c_str());

    //wstring wide_char = a2w_cp(str, CP_UTF8);
    //string multi_char = w2a(wide_char);

    //str = multi_char;

    return (UINT)str.size();
}

int Rtsp::Write(string str)
{
    int iWrite;

    printf(">> '%s'", str.c_str());

    //wstring wide_char = a2w(str);
    //string multi_utf8 = w2a_cp(wide_char, CP_UTF8);

    iWrite = Tcp::Write((PBYTE)str.c_str(), str.length());
    Tcp::Write((PBYTE)"\r\n", 2);

    printf("done.\n");

    return iWrite;
}

void Rtsp::AddField(string field)
{
    m_Fields.push_back(field);
}

void Rtsp::WriteFields()
{
    for(UINT iField = 0; iField < m_Fields.size(); iField++)
    {
        Write(m_Fields[iField]);
    }
    m_Fields.clear();
}

BOOL Rtsp::ParseMrl(string mrl, string* pPreSuffix, string* pSuffix, int* pPort)
{
    int port;
    string preSuffix;
    string suffix;
    string::size_type iFind, iFindEnd;

    iFind = mrl.find("rtsp://");
    if(iFind == string::npos)
    {
        //printf("rtsp: bad url: %s\n", mrl);
        return FALSE;
    }

    mrl.erase(0, iFind + 7);	//remove "rtsp://"

    port = RTSP_PROTOCOL_PORT;	//标准rtsp协议端口
    iFind = mrl.find(':');
    if(iFind != string::npos)
    {
        iFindEnd = mrl.find('/', iFind);
        if(iFindEnd != string::npos)
        {
            port = atoi(mrl.substr(iFind + 1, iFindEnd - iFind).c_str());
            mrl.erase(iFind, iFindEnd - iFind);
        }
    }

    iFind = mrl.find('/');
    if(iFind != string::npos)
    {
        preSuffix = mrl.substr(0, iFind - 0);
        mrl.erase(0, iFind + 1);
        suffix = mrl;
    }
    else
    {
        preSuffix = mrl;
    }

    if(pPreSuffix)
        *pPreSuffix = preSuffix;

    if(pSuffix)
        *pSuffix = suffix;

    if(pPort)
        *pPort = port;

    return TRUE;
}

void Rtsp::initSdt()
{
    sdt.RR.head = 0x81;
    sdt.RR.PT = 0xc9;
    sdt.RR.length[0] = 0x00;
    sdt.RR.length[1] = 0x07;
    memcpy(sdt.RR.SSRC, &ssrc, 4);
    ssrc++;
    sdt.RR.fractionLost = 0;
    memset(sdt.RR.cumulationLost, 0xff, 3);
    memset(sdt.RR.EHSNR, 0, 4);
    memset(sdt.RR.interJitter, 0, 4);
    memset(sdt.RR.LSR, 0, 4);
    memset(sdt.RR.DLSR, 0, 4);

    sdt.SDES.head = 0x81;
    sdt.SDES.PT = 0xCA;
    sdt.SDES.length[0] = 0x00;
    sdt.SDES.length[1] = 0x06;
    memcpy(sdt.SDES.SSRC, sdt.RR.SSRC, 4);
    sdt.SDES.user[0] = 0x01;
    sdt.SDES.user[1] = 0x11;
    char userMsg[17] = "unuseless ending";
    memcpy(sdt.SDES.user + 2, userMsg, 17);
    sdt.SDES.user[19] = 0;
}

int Rtsp::Read_Test()
{
    char buffer[1000];
    CSocket::Read((BYTE*)buffer, 1000);
    return 0;
}