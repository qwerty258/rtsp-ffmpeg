#include "PlayClient.h"
#include <atlstr.h>
#include <string>
#include <Windows.h>

// thread function
DWORD WINAPI RTSPVideo(LPVOID lpParam)
{
#ifdef _DEBUG // thread log
    FILE* pFile = fopen("D:\\thread.log", "ab");
    char temp[1024];
    sprintf(temp, "rtsp instance: %d, %p Created\n", static_cast<CRTSPClient*>(lpParam)->m_decode_instance, static_cast<CRTSPClient*>(lpParam)->m_hThread);
    fwrite(temp, 1, strlen(temp), pFile);
    fclose(pFile);
#endif // thread log end

    //RTSPCLient->m_SetupName_audio = "";
    //RTSPCLient->m_SetupName_video = "";
    if(!static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->RequestPlay())
    {
        //clean up on failure
        static_cast<CRTSPClient*>(lpParam)->m_ans = 4;
        return -1;
    }

    if(static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->frame != -1)
    {
        static_cast<CRTSPClient*>(lpParam)->m_myparamInput->fps = static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->frame;
    }

    initial_decode_parameter(
        static_cast<CRTSPClient*>(lpParam)->m_decode_instance,
        static_cast<CRTSPClient*>(lpParam)->m_myparamInput,
        static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->encoding_type);


    static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->ID = static_cast<CRTSPClient*>(lpParam)->m_decode_instance;
    static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->nfirst = true;
    static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->initS = 0;

    if(static_cast<CRTSPClient*>(lpParam)->m_circulation)
    {
        return -1; // already playing
    }

    static_cast<CRTSPClient*>(lpParam)->m_circulation = true;
    DWORD time1 = GetTickCount();
    DWORD time2 = GetTickCount();

    static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->initSdt();

    while(static_cast<CRTSPClient*>(lpParam)->m_circulation)
    {
        static_cast<CRTSPClient*>(lpParam)->m_ans = 1;

        int type = 0;
        short int size = 0;

        if(-1 == static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->Read_Start(type, &size))
        {
            continue;
        }

        if(size < 12)
            continue;
        if(type == 0)
        {
            int i = static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->Read_PlayLoad(size);
            if(!static_cast<CRTSPClient*>(lpParam)->m_bPause)
            {
                if(static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->encoding_type == 1)
                {
                    static_cast<CRTSPClient*>(lpParam)->m_p_CRTPPackage->set_RTP_package_buffer(
                        static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->m_p_RTP_package_buffer,
                        size);
                    static_cast<CRTSPClient*>(lpParam)->m_p_CRTPPackage->unpack_RTP_header();
                    static_cast<CRTSPClient*>(lpParam)->m_p_CRTPPackage->unpack_H264_NAL_header(
                        &static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->nfirst);

                    decode(
                        static_cast<CRTSPClient*>(lpParam)->m_decode_instance,
                        static_cast<CRTSPClient*>(lpParam)->m_p_CRTPPackage->m_p_unpack_result,
                        static_cast<CRTSPClient*>(lpParam)->m_p_CRTPPackage->m_unpack_result_size,
                        static_cast<CRTSPClient*>(lpParam)->m_p_CRTPPackage->m_p_RTP_header->sequence_number,
                        static_cast<CRTSPClient*>(lpParam)->m_p_CRTPPackage->m_p_RTP_header->timestamp);

                }
                if(static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->encoding_type == 2)
                {
                    static_cast<CRTSPClient*>(lpParam)->m_p_CRTPPackage->set_RTP_package_buffer(
                        static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->m_p_RTP_package_buffer,
                        size);
                    static_cast<CRTSPClient*>(lpParam)->m_p_CRTPPackage->unpack_RTP_header();
                    static_cast<CRTSPClient*>(lpParam)->m_p_CRTPPackage->unpack_MPEG(
                        &static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->nfirst);

                    decode(
                        static_cast<CRTSPClient*>(lpParam)->m_decode_instance,
                        static_cast<CRTSPClient*>(lpParam)->m_p_CRTPPackage->m_p_unpack_result,
                        static_cast<CRTSPClient*>(lpParam)->m_p_CRTPPackage->m_unpack_result_size,
                        static_cast<CRTSPClient*>(lpParam)->m_p_CRTPPackage->m_p_RTP_header->sequence_number,
                        static_cast<CRTSPClient*>(lpParam)->m_p_CRTPPackage->m_p_RTP_header->timestamp);

                }
            }
        }
        else if(type == 2 || type == 3) // discard rest of streams(2-3)
        {
            static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->Read_Leave(size);
        }
        else if(type == 1)
        {
            static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->Read_RTCPVideo(size);
        }


        // RTCP module, send data once every 5 second, interval can be adjusted according to bandwidth
        time2 = GetTickCount();
        if(time2 - time1 > 5000)
        {
            time1 = time2;
            //fraction lost
            char eTmp[2];
            eTmp[0] = static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->eSeNum[1];
            eTmp[1] = static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->eSeNum[0];

            char lTmp[2];
            lTmp[0] = static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->lSeNum[1];
            lTmp[1] = static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->lSeNum[0];

            static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->sdt.RR.fractionLost = (UINT)((float)(((UINT16)eTmp - (UINT16)lTmp) - static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->perGet) / (float)((UINT16)eTmp - (UINT16)lTmp) * 256);

            static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->perGet = 0;
            memcpy(
                static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->lSeNum,
                static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->eSeNum,
                2);

            //cumulation lost
            char sTmp[2];
            sTmp[0] = static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->sSeNum[1];
            sTmp[1] = static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->sSeNum[0];

            UINT16 lost = (UINT16)eTmp - (UINT16)sTmp - static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->allGet;

            char l[2];

            memcpy(l + 1, &lost, 1);
            memcpy(l, &lost + 1, 1);

            memcpy(static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->sdt.RR.cumulationLost + 1, l, 2);
            //interarrival jitter
            char jit[4];
            memcpy(jit, &static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->jitter + 3, 1);
            memcpy(jit + 1, &static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->jitter + 2, 1);
            memcpy(jit + 2, &static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->jitter + 1, 1);
            memcpy(jit + 3, &static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->jitter + 0, 1);

            memcpy(static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->sdt.RR.interJitter, jit, 4);
            //last SR
            memcpy(static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->sdt.RR.LSR, static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->LSR, 4);

            //DLSR
            time_t nowTime;
            time(&nowTime);

            DWORD delayTime = nowTime - static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->lTime;

            char dTime[4];
            memcpy(dTime, &delayTime + 3, 1);
            memcpy(dTime + 1, &delayTime + 2, 1);
            memcpy(dTime + 2, &delayTime + 1, 1);
            memcpy(dTime + 3, &delayTime, 1);

            memcpy(static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->sdt.RR.DLSR, dTime, 4);

            memcpy(static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->sdt.RR.SSRC_1, static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->rcvf.SR.SSRC, 4);

            static_cast<CRTSPClient*>(lpParam)->m_RTSPRequest->Write_RTCPVideo(0);
        }
    }

    // close decode
    if(0 > free_decode_instance(static_cast<CRTSPClient*>(lpParam)->m_decode_instance))
    {
        return -1;
    }

    static_cast<CRTSPClient*>(lpParam)->m_ans = 2;

#ifdef _DEBUG // thread log
    pFile = fopen("D:\\thread.log", "ab");
    sprintf(temp, "rtsp instance: %d, %p end\n", static_cast<CRTSPClient*>(lpParam)->m_decode_instance, static_cast<CRTSPClient*>(lpParam)->m_hThread);
    fwrite(temp, 1, strlen(temp), pFile);
    fclose(pFile);
#endif // thread log end

    return 0;
}

CRTSPClient::CRTSPClient()
{
    m_bInitURI = false;
    m_bConnected = false;
    m_bPlaying = false;

    m_URI = new char[256];
    m_userName = new char[256];
    m_password = new char[256];

    memset(m_URI, 0x0, 256);
    memset(m_userName, 0x0, 256);
    memset(m_password, 0x0, 256);

    m_threadID = -1;
    m_circulation = false;
    m_decode_instance = -1;
    m_hWnd = NULL;
    m_ans = 0;

    m_RTSPRequest = new CRTSPRequest;

    m_myparamInput = new myparamInput;

    m_p_CRTPPackage = new CRTPPackage;
}

CRTSPClient::~CRTSPClient()
{
    if(NULL != m_URI)
    {
        delete[] m_URI;
        m_URI = NULL;
    }

    if(NULL != m_userName)
    {
        delete[] m_userName;
        m_userName = NULL;
    }

    if(NULL != m_password)
    {
        delete[] m_password;
        m_password = NULL;
    }

    if(NULL != m_RTSPRequest)
    {
        delete m_RTSPRequest;
        m_RTSPRequest = NULL;
    }

    if(NULL != m_myparamInput)
    {
        delete m_myparamInput;
        m_myparamInput = NULL;
    }

    if(NULL != m_p_CRTPPackage)
    {
        delete m_p_CRTPPackage;
        m_p_CRTPPackage = NULL;
    }
}

int CRTSPClient::input_URI(char* URI, char* username, char* password, HWND hWnd)
{
    if(m_bInitURI || m_bConnected || m_bPlaying || m_bPause || NULL == username || NULL == password || NULL == URI)
    {
        return -1;
    }

    strncpy(m_URI, URI, 256);
    strncpy(m_userName, username, 256);
    strncpy(m_password, password, 256);

    m_hWnd = hWnd;

    // prepare data for decode

    RECT rect;
    GetWindowRect(m_hWnd, &rect);
    m_myparamInput->playHandle = m_hWnd; // get controls' window handle
    m_myparamInput->stopPlay = 0;
    m_myparamInput->playChannle = 1;
    m_myparamInput->fps = 25;
    m_myparamInput->isDecode = true;
    m_myparamInput->playHeight = rect.bottom - rect.top;
    m_myparamInput->playWidth = rect.right - rect.left;

    m_bInitURI = true;

    return 0;
}

int CRTSPClient::connect(void)
{
    if(!m_bInitURI || m_bConnected || m_bPlaying || m_bPause)
    {
        return -1;
    }

    // set up communication port
    string setupName = "";
    srand(time(NULL));
    static int initPort = rand() % 8000;
    int rtpPort = 2000 + 6 * initPort;
    initPort++;
    int rtcpPort = rtpPort + 1;
    string sdp = "";
    char* sess = 0;

    // get local IP
    string ip;
    WORD wVersionRequested;
    WSADATA wsaData; // initial winsock
    char name[255];
    memset(name, 0x0, 255);

    PHOSTENT hostinfo = NULL;
    wVersionRequested = MAKEWORD(2, 0);

    if(WSAStartup(wVersionRequested, &wsaData) == 0)
    {

        if(gethostname(name, sizeof(name)) == 0)
        {
            if((hostinfo = (PHOSTENT)gethostbyname(name)) != NULL)
            {
                ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
            }
        }

        WSACleanup();
    }

    if(!m_RTSPRequest->Open(m_URI, ip.c_str(), rtpPort + 5))
    {
        m_ans = 4;
        return -1;
    }

    if(!m_RTSPRequest->RequestOptions())
    {
        if(!m_RTSPRequest->RequestOptions(m_userName, m_password))
        {
            m_ans = 4;
            return -1;
        }
    }

    if(!m_RTSPRequest->RequestDescribe(&sdp)) // sometimes it fails
    {
        if(!m_RTSPRequest->RequestDescribe(&sdp, m_userName, m_password))
        {
            m_ans = 4;
            return -1;
        }
    }

#ifdef MY_DEBUG
    FILE* pFile = fopen("D:\\sdp.log", "w");
    fwrite(sdp.c_str(), 1, sdp.length(), pFile);
    fclose(pFile);
#endif


    // video
    if(m_RTSPRequest->m_SetupName_video.length())
    {
        m_RTSPRequest->m_SetupName = m_RTSPRequest->m_SetupName_video;
        if(!m_RTSPRequest->RequestSetup(setupName.c_str(), transportModeRtpTcp, 0, 1, sess))
        {
            if(!m_RTSPRequest->RequestSetup(setupName.c_str(), transportModeRtpTcp, 0, 1, sess, m_userName, m_password))
            {
                m_ans = 4;
                return -1;
            }
        }
    }

    //audio
    if(m_RTSPRequest->m_SetupName_audio.length())
    {
        m_RTSPRequest->m_SetupName = m_RTSPRequest->m_SetupName_audio;
        if(!m_RTSPRequest->RequestSetup(setupName.c_str(), transportModeRtpTcp, 2, 3, sess))
        {
            if(!m_RTSPRequest->RequestSetup(setupName.c_str(), transportModeRtpTcp, 2, 3, sess, m_userName, m_password))
            {
                m_ans = 4;
                return -1;
            }
        }
    }

    m_RTSPRequest->m_SetupName = "";

    m_bConnected = true;

    return 0;
}

//**************************************************
//function    : realtime play
//input       :
//output      :
//return value: 1 success, -1 failure
//**************************************************
int CRTSPClient::play(void)
{
    if(!m_bInitURI || !m_bConnected || m_bPlaying)
    {
        return -1;
    }

    if(m_bPause)
    {
        m_bPause = false;
        m_bPlaying = true;
    }
    else
    {
        // enter the thread
        m_hThread = CreateThread(NULL, 0, RTSPVideo, this, 0, &m_threadID);
        m_bPlaying = true;

        if(m_threadID == -1 || m_ans == 4)
        {
            return -1; // enter thread failed
        }

    }

    return 0;
}

int CRTSPClient::pause(void)
{
    if(!m_bInitURI || !m_bConnected || !m_bPlaying || m_bPause)
    {
        return -1;
    }

    m_bPause = true;
    m_bPlaying = false;

    return 0;
}

//**************************************************
//function    : close realtime play
//input       :
//output      :
//return value: 1 success, -1 failure
//**************************************************
int CRTSPClient::disconnect(void)
{
    if(!m_bInitURI || !m_bConnected)
    {
        return -1;
    }

    m_circulation = false;

    if(!m_RTSPRequest->RequestTeardown(m_userName, m_password))
    {
        return -1;
    }

    m_bConnected = false;
    m_bInitURI = false;
    m_bPlaying = false;
    m_bPause = false;

    return 0;
}
