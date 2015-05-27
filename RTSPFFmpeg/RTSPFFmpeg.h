#pragma once
#ifdef RTSPFFMPEG_EXPORTS
#define RTSPFFMPEG_API __declspec(dllexport)
#else
#define RTSPFFMPEG_API __declspec(dllimport)
#endif

#include "PlayClient.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    // function    : initial RTSPFFmpeg.dll
    // input       :
    // output      :
    // return value: 0 success, -1 failure
    RTSPFFMPEG_API int InitRtspDLL(void);

    // function    : uninitial RTSPFFmpeg.dll, release resource
    // input       :
    // output      :
    // return value: 0 success, -1 failure
    RTSPFFMPEG_API int FreeRtspDLL(void);

    // function    : return idle instance
    // input       :
    // output      :
    // return value: idle instance number on success, -1 on failure
    RTSPFFMPEG_API int GetRtspINSTANCE(void);

    // function    : set IPC's URI, username and password
    // input       :
    //// INSTANCE  : the idle instance return by GetRtspINSTANCE
    //// URI       : IPC's URI(RTSP)
    //// userName  : IPC's username
    //// password  : IPC's password
    // output      :
    // return value: 0 success, -1 failure
    RTSPFFMPEG_API int InitRtspVideoParam(int INSTANCE, char* URI, char* userName, char* password);

    // function    : connect to the IPC, you must have idle instance and initial IPC's video parameter
    // input       :
    //// INSTANCE  : the idle instance return by GetRtspINSTANCE
    // output      :
    // return value: 0 success, -1 failure
    RTSPFFMPEG_API int Connect(int INSTANCE);

    // function    : play realtime feedback of the IPC
    // input       :
    //// INSTANCE  : the idle instance return by GetRtspINSTANCE
    //// hWnd      : handle to the display window
    // output      :
    // return value: 0 success, -1 failure
    RTSPFFMPEG_API int Play(int INSTANCE, HWND hWnd);

    // function    : pause realtime play
    // input       :
    //// INSTANCE  : the instance playing
    // output      :
    // return value: 0 success, -1 failure
    RTSPFFMPEG_API int Pause(int INSTANCE);

    RTSPFFMPEG_API int DisConnect(int INSTANCE);

    RTSPFFMPEG_API int FreeRtspDLL0000(int INSTANCE);
    RTSPFFMPEG_API int pSetDrawLineCallBack(int INSTANCE, TDrawLineCallBack f1);
    RTSPFFMPEG_API int pSetPFCALLBACK(int INSTANCE, PFCALLBACK f1);
    RTSPFFMPEG_API int pSetBmpCallBack(int INSTANCE, TBmpCallBack f2);
    RTSPFFMPEG_API int pSetFillBmpCallBack(int INSTANCE, TDrawRectCallBack f3);
    //获取YUV数据
    RTSPFFMPEG_API int pSetYUVCallBack(int INSTANCE, TYUVCallBack f3, void *buffer);
    //获取H264数据
    RTSPFFMPEG_API int pSetH264CallBack(int INSTANCE, TH264CallBack f3);
    //设置是否取消硬解
    RTSPFFMPEG_API int RevoHWAcceleration(int INSTANCE);

#ifdef __cplusplus
}
#endif // __cplusplus