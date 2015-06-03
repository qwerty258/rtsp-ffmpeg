#include "PlayClient.h"
#include <string>
#include <atlstr.h>
#include <fstream>
#include <sstream>
#include "Rtcp.h"
#include <Windows.h>

// thread function
DWORD WINAPI RTSPVideo(LPVOID lpParam)
{
    // prepare data for decode
    RECT *rect = new RECT; // delete when play end
    GetWindowRect(((CRTSPCLient*)lpParam)->m_hWnd, rect);
    ((CRTSPCLient*)lpParam)->m_myparamInput->playHandle = ((CRTSPCLient*)lpParam)->m_hWnd; // get controls' window handle
    ((CRTSPCLient*)lpParam)->m_myparamInput->stopPlay = 0;
    ((CRTSPCLient*)lpParam)->m_myparamInput->playChannle = 1;
    ((CRTSPCLient*)lpParam)->m_myparamInput->fps = 25;
    ((CRTSPCLient*)lpParam)->m_myparamInput->isDecode = true;
    ((CRTSPCLient*)lpParam)->m_myparamInput->playHeight = rect->bottom - rect->top;
    ((CRTSPCLient*)lpParam)->m_myparamInput->playWidth = rect->right - rect->left;

    //RTSPCLient->m_SetupName_audio = "";
    //RTSPCLient->m_SetupName_video = "";
    if(!((CRTSPCLient*)lpParam)->m_RTSPRequest->RequestPlay())
    {
        //clean up on failure
        if(rect != NULL)
        {
            delete rect;
            rect = NULL;
        }

        ((CRTSPCLient*)lpParam)->m_ans = 4;
        return -1;
    }

    if(((CRTSPCLient*)lpParam)->m_RTSPRequest->frame != -1)
    {
        ((CRTSPCLient*)lpParam)->m_myparamInput->fps = ((CRTSPCLient*)lpParam)->m_RTSPRequest->frame;
    }

    Sleep(10);
    ((CRTSPCLient*)lpParam)->m_p_func_InitVideoParam(((CRTSPCLient*)lpParam)->m_INSTANCE, ((CRTSPCLient*)lpParam)->m_myparamInput, ((CRTSPCLient*)lpParam)->m_RTSPRequest->Decode);
    // set callback
    ((CRTSPCLient*)lpParam)->m_p_func_SetCallBack(((CRTSPCLient*)lpParam)->m_INSTANCE, ((CRTSPCLient*)lpParam)->func);
    ((CRTSPCLient*)lpParam)->m_p_func_SetDrawLineCallBack(((CRTSPCLient*)lpParam)->m_INSTANCE, ((CRTSPCLient*)lpParam)->funcD);
    ((CRTSPCLient*)lpParam)->m_p_func_SetBmpCallBack(((CRTSPCLient*)lpParam)->m_INSTANCE, ((CRTSPCLient*)lpParam)->bmpFunc);
    ((CRTSPCLient*)lpParam)->m_p_func_SetFillBmpCallBack(((CRTSPCLient*)lpParam)->m_INSTANCE, ((CRTSPCLient*)lpParam)->fillbmp);
    ((CRTSPCLient*)lpParam)->m_p_func_setYUVCallBack(((CRTSPCLient*)lpParam)->m_INSTANCE, ((CRTSPCLient*)lpParam)->YUVFunc, ((CRTSPCLient*)lpParam)->YUVEx);
    ((CRTSPCLient*)lpParam)->m_p_func_setH264CallBack(((CRTSPCLient*)lpParam)->m_INSTANCE, ((CRTSPCLient*)lpParam)->H264Func);

    if(!((CRTSPCLient*)lpParam)->nHWAcceleration)
    {
        ((CRTSPCLient*)lpParam)->m_p_func_revoHWFunc(((CRTSPCLient*)lpParam)->m_INSTANCE);
    }

    ((CRTSPCLient*)lpParam)->m_RTSPRequest->ID = ((CRTSPCLient*)lpParam)->m_INSTANCE;
    ((CRTSPCLient*)lpParam)->m_RTSPRequest->nfirst = true;
    ((CRTSPCLient*)lpParam)->m_RTSPRequest->initS = 0;

    if(((CRTSPCLient*)lpParam)->m_circulation)
    {
        return -1; // already playing
    }

    ((CRTSPCLient*)lpParam)->m_circulation = true;
    DWORD time1 = GetTickCount();
    DWORD time2 = GetTickCount();

    ((CRTSPCLient*)lpParam)->m_RTSPRequest->initSdt();

    while(((CRTSPCLient*)lpParam)->m_circulation)
    {
        ((CRTSPCLient*)lpParam)->m_ans = 1;

        int type = 0;
        short int size = 0;

        int rs = ((CRTSPCLient*)lpParam)->m_RTSPRequest->Read_Start(type, &size);


        if(rs == -1)
            continue;

        if(size < 12)
            continue;
        if(type == 0)
        {
            int i = ((CRTSPCLient*)lpParam)->m_RTSPRequest->Read_PlayLoad(size);
        }
        else if(type == 2 || type == 3) // discard rest of streams(2-3)
        {
            ((CRTSPCLient*)lpParam)->m_RTSPRequest->Read_Leave(size);
        }
        else if(type == 1)
        {
            ((CRTSPCLient*)lpParam)->m_RTSPRequest->Read_RTCPVideo(size);
        }


        // RTCP module, send data once every 5 second, interval can be adjusted according to bandwidth
        time2 = GetTickCount();
        if(time2 - time1 > 5000)
        {
            time1 = time2;
            //fraction lost
            char eTmp[2];
            eTmp[0] = ((CRTSPCLient*)lpParam)->m_RTSPRequest->eSeNum[1];
            eTmp[1] = ((CRTSPCLient*)lpParam)->m_RTSPRequest->eSeNum[0];

            char lTmp[2];
            lTmp[0] = ((CRTSPCLient*)lpParam)->m_RTSPRequest->lSeNum[1];
            lTmp[1] = ((CRTSPCLient*)lpParam)->m_RTSPRequest->lSeNum[0];

            ((CRTSPCLient*)lpParam)->m_RTSPRequest->sdt.RR.fractionLost = (UINT)((float)(((UINT16)eTmp - (UINT16)lTmp) - ((CRTSPCLient*)lpParam)->m_RTSPRequest->perGet) / (float)((UINT16)eTmp - (UINT16)lTmp) * 256);

            ((CRTSPCLient*)lpParam)->m_RTSPRequest->perGet = 0;
            memcpy(((CRTSPCLient*)lpParam)->m_RTSPRequest->lSeNum, ((CRTSPCLient*)lpParam)->m_RTSPRequest->eSeNum, 2);
            //cumulation lost
            char sTmp[2];
            sTmp[0] = ((CRTSPCLient*)lpParam)->m_RTSPRequest->sSeNum[1];
            sTmp[1] = ((CRTSPCLient*)lpParam)->m_RTSPRequest->sSeNum[0];

            UINT16 lost = (UINT16)eTmp - (UINT16)sTmp - ((CRTSPCLient*)lpParam)->m_RTSPRequest->allGet;

            char l[2];

            memcpy(l + 1, &lost, 1);
            memcpy(l, &lost + 1, 1);

            memcpy(((CRTSPCLient*)lpParam)->m_RTSPRequest->sdt.RR.cumulationLost + 1, l, 2);
            //interarrival jitter
            char jit[4];
            memcpy(jit, &((CRTSPCLient*)lpParam)->m_RTSPRequest->jitter + 3, 1);
            memcpy(jit + 1, &((CRTSPCLient*)lpParam)->m_RTSPRequest->jitter + 2, 1);
            memcpy(jit + 2, &((CRTSPCLient*)lpParam)->m_RTSPRequest->jitter + 1, 1);
            memcpy(jit + 3, &((CRTSPCLient*)lpParam)->m_RTSPRequest->jitter + 0, 1);

            memcpy(((CRTSPCLient*)lpParam)->m_RTSPRequest->sdt.RR.interJitter, jit, 4);
            //last SR
            memcpy(((CRTSPCLient*)lpParam)->m_RTSPRequest->sdt.RR.LSR, ((CRTSPCLient*)lpParam)->m_RTSPRequest->LSR, 4);

            //DLSR
            time_t nowTime;
            time(&nowTime);

            DWORD delayTime = nowTime - ((CRTSPCLient*)lpParam)->m_RTSPRequest->lTime;

            char dTime[4];
            memcpy(dTime, &delayTime + 3, 1);
            memcpy(dTime + 1, &delayTime + 2, 1);
            memcpy(dTime + 2, &delayTime + 1, 1);
            memcpy(dTime + 3, &delayTime, 1);

            memcpy(((CRTSPCLient*)lpParam)->m_RTSPRequest->sdt.RR.DLSR, dTime, 4);

            memcpy(((CRTSPCLient*)lpParam)->m_RTSPRequest->sdt.RR.SSRC_1, ((CRTSPCLient*)lpParam)->m_RTSPRequest->rcvf.SR.SSRC, 4);

            ((CRTSPCLient*)lpParam)->m_RTSPRequest->Write_RTCPVideo(0);
        }
    }

    // close decode
    int ret = ((CRTSPCLient*)lpParam)->m_p_func_freeVideos(((CRTSPCLient*)lpParam)->m_INSTANCE);
    if(ret < 0)
        return -1;

    if(rect != NULL)
    {
        delete rect;
        rect = NULL;
    }

    ((CRTSPCLient*)lpParam)->m_ans = 2;

    return 0;
}

CRTSPCLient::CRTSPCLient()
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
    m_INSTANCE = -1;
    m_hWnd = NULL;
    m_ans = 0;

    m_RTSPRequest = new CRTSPRequest;

    m_myparamInput = new myparamInput;

    func = NULL;
    funcD = NULL;
    bmpFunc = NULL;
    fillbmp = NULL;
    YUVFunc = NULL;
    H264Func = NULL;
    YUVEx = NULL;
    nHWAcceleration = false;
}

CRTSPCLient::~CRTSPCLient()
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
}

int CRTSPCLient::input_URI(char* URI, char* username, char* password)
{
    if(m_bInitURI || m_bConnected || m_bPlaying || m_RTSPRequest->m_CRTSP_paused || NULL == username || NULL == password || NULL == URI)
    {
        return -1;
    }

    strncpy(m_URI, URI, 256);
    strncpy(m_userName, username, 256);
    strncpy(m_password, password, 256);

    m_bInitURI = true;

    return 0;
}

int CRTSPCLient::connect()
{
    if(!m_bInitURI || m_bConnected || m_bPlaying || m_RTSPRequest->m_CRTSP_paused)
    {
        return -1;
    }

    m_INSTANCE = m_p_func_GetIdlevideoINSTANCE();

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
int CRTSPCLient::play(HWND hWnd)
{
    if(!m_bInitURI || !m_bConnected || m_bPlaying)
    {
        return -1;
    }

    if(m_RTSPRequest->m_CRTSP_paused)
    {
        m_RTSPRequest->m_CRTSP_paused = false;
        m_bPlaying = true;
    }
    else
    {
        // enter the thread
        m_hWnd = hWnd;
        CloseHandle(CreateThread(NULL, 0, RTSPVideo, this, 0, &m_threadID));

        m_bPlaying = true;

        //while(m_ans != 1 && m_ans != 4)
        //{
        //    Sleep(1);
        //}

        if(m_threadID == -1 || m_ans == 4)
        {
            return -1; // enter thread failed
        }

    }

    return 0;
}

int CRTSPCLient::pause()
{
    if(!m_bInitURI || !m_bConnected || !m_bPlaying || m_RTSPRequest->m_CRTSP_paused)
    {
        return -1;
    }

    m_RTSPRequest->m_CRTSP_paused = true;
    m_bPlaying = false;

    return 0;
}

//**************************************************
//function    : close realtime play
//input       :
//output      :
//return value: 1 success, -1 failure
//**************************************************
int CRTSPCLient::disconnect()
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

    //while(m_ans != 2 && m_ans != 4)
    //{
    //    Sleep(1);
    //}

    m_bConnected = false;
    m_bInitURI = false;
    m_bPlaying = false;
    m_RTSPRequest->m_CRTSP_paused = false;

    return 0;
}
