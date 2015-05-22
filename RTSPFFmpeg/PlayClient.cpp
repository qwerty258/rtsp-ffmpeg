#define _CRT_SECURE_NO_WARNINGS
#include "PlayClient.h"
#include <string>
#include <atlstr.h>
#include <fstream>
#include <sstream>
#include "Rtcp.h"
#include <Windows.h>

//#define log

bool inited = false;
setYUVCallBack setYUVCallBackFunc;
setH264CallBack setH264CallBackFunc;
fSetCallBack SetCallBack;
finitVideoDLL initVideoDLL;
fGetIdlevideoINSTANCE GetIdlevideoINSTANCE;
fInitVideoParamNew InitVideoParamNew;
fInitVideoParam InitVideoParam;
fpauseVideos pauseVideos;
fplayVideos playVideos;
ffreeVideos freeVideos;
finputBuf inputBuf;
fresize resize;
fexitdll exitdll;
fSetDrawLineCallBack SetDrawLineCallBack;
fSetBmpCallBack SetBmpCallBack;
fSetFillBmpCallBack SetFillBmpCallBack;
revoHW revoHWFunc;

CRTSPCLient::CRTSPCLient():m_URI(NULL), m_userName(NULL), m_password(NULL)
{
    m_URI = new char[256];
    m_userName = new char[256];
    m_password = new char[256];

    m_threadID = -1;
    m_circulation = false;
    m_INSTANCE = -1;
    m_hWnd = NULL;
    m_ans = 0;

    m_RTSPRequest = new CRTSPRequest;

    func = NULL;
    funcD = NULL;
    bmpFunc = NULL;
    fillbmp = NULL;
    YUVFunc = NULL;
    H264Func = NULL;
    YUVEx = NULL;
    nHWAcceleration = false;

    m_hDLL = LoadLibrary(L"PlayH264DLL.dll");
    if(NULL == m_hDLL)
    {
        TCHAR* temp = new TCHAR[2048];
        wsprintfW(temp, L"LoadLibrary PlayH24DLL.dll error, error code: %d", GetLastError());
        MessageBox(0, temp, NULL, MB_OK);
        delete[] temp;
    }

    initVideoDLL = (finitVideoDLL)GetProcAddress(m_hDLL, "initVideoDLL");
    if(NULL == initVideoDLL)
    {
        TCHAR* temp = new TCHAR[2048];
        wsprintfW(temp, L"GetProcAddress initVideoDLL error, error code: %d", GetLastError());
        MessageBox(0, temp, NULL, MB_OK);
        delete[] temp;
    }
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

    FreeLibrary(m_hDLL);
}

int CRTSPCLient::InputURL(char* URI, char* userName, char* password)
{
    strncpy(m_userName, userName, 256);
    strncpy(m_password, password, 256);
    strncpy(m_URI, URI, 256);

    if(!inited)
    {
        initVideoDLL();
        inited = true;
    }

    return 1;
}

//线程函数
DWORD WINAPI RTSPVideo(LPVOID lpParam)
{
    //解码器准备工�?
    myparamInput *Myparam = new myparamInput();//播放结束需要删�?
    RECT *rect = new RECT;//播放结束需要删�?
    GetWindowRect(((CRTSPCLient*)lpParam)->m_hWnd, rect);
    Myparam->playHandle = ((CRTSPCLient*)lpParam)->m_hWnd;  // 取得控件的句�?
    Myparam->stopPlay = 0;
    Myparam->playChannle = 1;
    Myparam->fps = 25;
    Myparam->isDecode = true;
    Myparam->playHeight = rect->bottom - rect->top;
    Myparam->playWidth = rect->right - rect->left;

    HINSTANCE hdll = LoadLibrary(L"PlayH264DLL.dll");
    if(NULL == hdll)
    {
        MessageBox(NULL, L"LoadLibrary PlayH264DLL.dll error", NULL, MB_OK);
        return -1;
    }

    GetIdlevideoINSTANCE = (fGetIdlevideoINSTANCE)GetProcAddress(hdll, "GetIdlevideoINSTANCE");
    if(NULL == GetIdlevideoINSTANCE)
    {
        MessageBox(NULL, L"GetProcAddress GetIdlevideoINSTANCE error", NULL, MB_OK);
        return -1;
    }

    ((CRTSPCLient*)lpParam)->m_INSTANCE = GetIdlevideoINSTANCE();

    //建立通信
    string setupName = "";
    srand((unsigned int)time(0));
    static int initPort = rand() % 8000;
    INT rtpPort = 2000 + 6 * initPort;
    initPort++;
    INT rtcpPort = rtpPort + 1;
    string sdp = "";
    char * sess = 0;

    //获取本地IP
    string ip;
    WORD wVersionRequested;
    WSADATA wsaData;//初始�?
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

    if(!((CRTSPCLient*)lpParam)->m_RTSPRequest->Open(((CRTSPCLient*)lpParam)->m_URI, ip.c_str(), rtpPort + 5))
    {
        //clean up on failure
        if(NULL != Myparam)
        {
            delete Myparam;
            Myparam = NULL;
        }

        if(NULL != rect)
        {
            delete rect;
            rect = NULL;
        }

        ((CRTSPCLient*)lpParam)->m_ans = 4;
        return -1;
    }

    if(!((CRTSPCLient*)lpParam)->m_RTSPRequest->RequestOptions())
    {
        if(!((CRTSPCLient*)lpParam)->m_RTSPRequest->RequestOptions(((CRTSPCLient*)lpParam)->m_userName, ((CRTSPCLient*)lpParam)->m_password))
        {
            //失败了也需要清�?
            if(Myparam != NULL)
            {
                delete Myparam;
                Myparam = NULL;
            }
            if(rect != NULL)
            {
                delete rect;
                rect = NULL;
            }

            ((CRTSPCLient*)lpParam)->m_ans = 4; return -1;
        }
    }

    if(!((CRTSPCLient*)lpParam)->m_RTSPRequest->RequestDescribe(&sdp))//有时候会连不�?
    {
        if(!((CRTSPCLient*)lpParam)->m_RTSPRequest->RequestDescribe(&sdp, ((CRTSPCLient*)lpParam)->m_userName, ((CRTSPCLient*)lpParam)->m_password))
        {

            //失败了也需要清�?
            if(Myparam != NULL)
            {
                delete Myparam;
                Myparam = NULL;
            }
            if(rect != NULL)
            {
                delete rect;
                rect = NULL;
            }

            ((CRTSPCLient*)lpParam)->m_ans = 4; return -1;
        }
    }

    if(((CRTSPCLient*)lpParam)->m_RTSPRequest->m_SetupName_video.length())
    {
        ((CRTSPCLient*)lpParam)->m_RTSPRequest->m_SetupName = ((CRTSPCLient*)lpParam)->m_RTSPRequest->m_SetupName_video;
        if(!((CRTSPCLient*)lpParam)->m_RTSPRequest->RequestSetup(setupName.c_str(), transportModeRtpTcp, 0, 1, sess))
        {
            if(!((CRTSPCLient*)lpParam)->m_RTSPRequest->RequestSetup(setupName.c_str(), transportModeRtpTcp, 0, 1, sess, ((CRTSPCLient*)lpParam)->m_userName, ((CRTSPCLient*)lpParam)->m_password))
            {

                //失败了也需要清�?
                if(Myparam != NULL)
                {
                    delete Myparam;
                    Myparam = NULL;
                }
                if(rect != NULL)
                {
                    delete rect;
                    rect = NULL;
                }

                ((CRTSPCLient*)lpParam)->m_ans = 4; return -1;
            }
        }
    }

    //audio
    if(((CRTSPCLient*)lpParam)->m_RTSPRequest->m_SetupName_audio.length())
    {
        ((CRTSPCLient*)lpParam)->m_RTSPRequest->m_SetupName = ((CRTSPCLient*)lpParam)->m_RTSPRequest->m_SetupName_audio;
        if(!((CRTSPCLient*)lpParam)->m_RTSPRequest->RequestSetup(setupName.c_str(), transportModeRtpTcp, 2, 3, sess))
        {
            if(!((CRTSPCLient*)lpParam)->m_RTSPRequest->RequestSetup(setupName.c_str(), transportModeRtpTcp, 2, 3, sess, ((CRTSPCLient*)lpParam)->m_userName, ((CRTSPCLient*)lpParam)->m_password))
            {

                //失败了也需要清�?
                if(Myparam != NULL)
                {
                    delete Myparam;
                    Myparam = NULL;
                }
                if(rect != NULL)
                {
                    delete rect;
                    rect = NULL;
                }

                ((CRTSPCLient*)lpParam)->m_ans = 4; return -1;
            }
        }
    }

    ((CRTSPCLient*)lpParam)->m_RTSPRequest->m_SetupName = "";
    //RTSPCLient->m_SetupName_audio = "";
    //RTSPCLient->m_SetupName_video = "";
    if(!((CRTSPCLient*)lpParam)->m_RTSPRequest->RequestPlay())
    {
        //失败了也需要清�?
        if(Myparam != NULL)
        {
            delete Myparam;
            Myparam = NULL;
        }
        if(rect != NULL)
        {
            delete rect;
            rect = NULL;
        }

        ((CRTSPCLient*)lpParam)->m_ans = 4; return -1;
    }


    //初始化解码器dll
    if(((CRTSPCLient*)lpParam)->m_RTSPRequest->frame != -1)
    {
        Myparam->fps = ((CRTSPCLient*)lpParam)->m_RTSPRequest->frame;
    }

    Sleep(10);
    InitVideoParam = (fInitVideoParam)GetProcAddress(hdll, "InitVideoParam");
    InitVideoParam(((CRTSPCLient*)lpParam)->m_INSTANCE, Myparam, ((CRTSPCLient*)lpParam)->m_RTSPRequest->Decode);
    //设置回调
    SetCallBack = (fSetCallBack)GetProcAddress(hdll, "SetCallBack");
    SetDrawLineCallBack = (fSetDrawLineCallBack)GetProcAddress(hdll, "SetDrawLineCallBack");
    SetBmpCallBack = (fSetBmpCallBack)GetProcAddress(hdll, "SetBmpCallBack");
    SetFillBmpCallBack = (fSetFillBmpCallBack)GetProcAddress(hdll, "SetFillBmpCallBack");
    setYUVCallBackFunc = (setYUVCallBack)GetProcAddress(hdll, "SetYUVCallBack");
    setH264CallBackFunc = (setH264CallBack)GetProcAddress(hdll, "SetH264CallBack");
    revoHWFunc = (revoHW)GetProcAddress(hdll, "RevoHWAcceleration");
    SetCallBack(((CRTSPCLient*)lpParam)->m_INSTANCE, ((CRTSPCLient*)lpParam)->func);
    SetDrawLineCallBack(((CRTSPCLient*)lpParam)->m_INSTANCE, ((CRTSPCLient*)lpParam)->funcD);
    SetBmpCallBack(((CRTSPCLient*)lpParam)->m_INSTANCE, ((CRTSPCLient*)lpParam)->bmpFunc);
    SetFillBmpCallBack(((CRTSPCLient*)lpParam)->m_INSTANCE, ((CRTSPCLient*)lpParam)->fillbmp);
    setYUVCallBackFunc(((CRTSPCLient*)lpParam)->m_INSTANCE, ((CRTSPCLient*)lpParam)->YUVFunc, ((CRTSPCLient*)lpParam)->YUVEx);
    setH264CallBackFunc(((CRTSPCLient*)lpParam)->m_INSTANCE, ((CRTSPCLient*)lpParam)->H264Func);
    if(!((CRTSPCLient*)lpParam)->nHWAcceleration)
        revoHWFunc(((CRTSPCLient*)lpParam)->m_INSTANCE);

    ((CRTSPCLient*)lpParam)->m_RTSPRequest->ID = ((CRTSPCLient*)lpParam)->m_INSTANCE;
    ((CRTSPCLient*)lpParam)->m_RTSPRequest->nfirst = true;
    ((CRTSPCLient*)lpParam)->m_RTSPRequest->initS = 0;

    if(((CRTSPCLient*)lpParam)->m_circulation)
        return -1;//已经在播放了

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
        else if(type == 2 || type == 3)//其余2-3的流全部丢弃
        {
            ((CRTSPCLient*)lpParam)->m_RTSPRequest->Read_Leave(size);
        }
        else if(type == 1)
        {
            ((CRTSPCLient*)lpParam)->m_RTSPRequest->Read_RTCPVideo(size);
        }


        //RTCP模块,每隔大约5s，发送一次数据，间隔时间可根据实际带宽进行调�?
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

    //关解�?
    freeVideos = (ffreeVideos)GetProcAddress(hdll, "freeVideos");
    int ret = freeVideos(((CRTSPCLient*)lpParam)->m_INSTANCE);
    if(ret < 0)
        return -1;

    if(Myparam != NULL)
    {
        delete Myparam;
        Myparam = NULL;
    }
    if(rect != NULL)
    {
        delete rect;
        rect = NULL;
    }

    ((CRTSPCLient*)lpParam)->m_ans = 2;

    return 1;
}

//
//typedef DWORD WINAPI (* beginrecv)(LPVOID lpParam);
//**************************************************
//函数功能：实时播�?
//输入参数�?
//输出参数�?
//返回值：  1播放成功�?1播放失败
//**************************************************
int CRTSPCLient::PlayURL(HWND hWnd)
{
    //进入线程
    this->m_hWnd = hWnd;
    CreateThread(NULL, 0, RTSPVideo, this, 0, &m_threadID);
    while(m_ans != 1 && m_ans != 4)
        Sleep(1);
    if(m_threadID == -1 || m_ans == 4)
        return -1;//进入线程失败
    return 1;
}

//**************************************************
//函数功能：关闭实时播�?
//输入参数�?
//输出参数�?
//返回值：  -1关闭失败�?关闭成功
//**************************************************
int CRTSPCLient::stopURL()
{
    m_circulation = false;
    while(m_ans != 2 && m_ans != 4)
        Sleep(1);

    return 1;
}
