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

    RTSPFFMPEG_API int GetRtspINSTANCE(void);
    RTSPFFMPEG_API int InitRtspVideoParam(int INSTANCE, char* URI, char* userName, char* password);
    RTSPFFMPEG_API int Connect(int INSTANCE);
    RTSPFFMPEG_API int Play(int INSTANCE, HWND hd);
    RTSPFFMPEG_API int Stop(int INSTANCE);
    RTSPFFMPEG_API int InitRtspDLL(void);
    RTSPFFMPEG_API int FreeRtspDLL(void);
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