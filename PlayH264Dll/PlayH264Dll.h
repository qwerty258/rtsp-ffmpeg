#pragma once
#ifdef PLAYH264DLL_EXPORTS
#define PLAYH264DLL_API __declspec(dllexport)
#else
#define PLAYH264DLL_API __declspec(dllimport)
#endif

#include "DecodeH264threadClass.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    PLAYH264DLL_API int SetCallBack(int INSTANCE, PFCALLBACK f1);
    PLAYH264DLL_API void DecodeVideo(int num, uint8_t * pInBuffer, int size);
    PLAYH264DLL_API int initVideoDLL(int max_number_of_playing_instance);
    PLAYH264DLL_API int GetIdlevideoINSTANCE();
    PLAYH264DLL_API int InitVideoParam(int INSTANCE, myparamInput *Myparam, int type);
    PLAYH264DLL_API int pauseVideos(int INSTANCE);
    PLAYH264DLL_API int playVideos(int INSTANCE);
    PLAYH264DLL_API int freeVideos(int INSTANCE);
    PLAYH264DLL_API int inputBuf(int INSTANCE, char *buf, int bufsize);
    PLAYH264DLL_API int resize(int INSTANCE, int width, int height);
    PLAYH264DLL_API void exitdll();
    PLAYH264DLL_API int SetDrawLineCallBack(int INSTANCE, TDrawLineCallBack f1);
    PLAYH264DLL_API int SetBmpCallBack(int INSTANCE, TBmpCallBack bmp1);
    PLAYH264DLL_API int SetFillBmpCallBack(int INSTANCE, TDrawRectCallBack bmpf);
    PLAYH264DLL_API int SetYUVCallBack(int INSTANCE, TYUVCallBack yuvf, void *buffer);
    PLAYH264DLL_API int SetH264CallBack(int INSTANCE, TH264CallBack yuvf);
    //support hardware acceleration, default is software decode.
    //hardware acceleration support h264 and YUV callback, software decode support all callback functions
    PLAYH264DLL_API int RevoHWAcceleration(int instance);

#ifdef __cplusplus
}
#endif // __cplusplus