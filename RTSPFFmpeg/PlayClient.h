#pragma once

#include <list>
#include <map>

#include "RtspRequest.h"
#include "Rtp.h"

#include "VBuffer.h"

#include "Decode.h"

class RTSPCLientClass
{
public:
    RTSPCLientClass();
    ~RTSPCLientClass();
private:
    DWORD threadID;
public:
    int InputURL(char* URL, char* UserName, char* PWD);
    int PlayURL(HWND hd);
    int stopURL();
public:
    char * Url;
    int INSTANCE;
    char* UserName;
    char* Pwd;
    bool circulation;
    int ans;//0 初始状态，1 进入循环，2 退出线程
    HWND hd;
    PFCALLBACK func;
    TDrawLineCallBack funcD;
    TBmpCallBack bmpFunc;
    TDrawRectCallBack fillbmp;
    TYUVCallBack YUVFunc;
    TH264CallBack H264Func;
    void * YUVEx;
    bool nHWAcceleration;
};

DWORD WINAPI RTSPVideo(LPVOID lpParam);