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

    PLAYH264DLL_API int initial_decode_DLL(int max_number_of_decoding_instance);

    PLAYH264DLL_API int free_decode_DLL(void);

    PLAYH264DLL_API int get_idle_instance(void);

    PLAYH264DLL_API int initial_decode_parameter(int instance, myparamInput* Myparam, int type);

    PLAYH264DLL_API int decode(int instance, uint8_t* pInBuffer, int size);

    PLAYH264DLL_API int free_decode_instance(int instance);

    PLAYH264DLL_API int set_YUV420_callback(int instance, function_YUV420 p_function_YUV420, void* additional_data, bool trace_lost_package);

    PLAYH264DLL_API int SetCallBack(int INSTANCE, PFCALLBACK f1);
    PLAYH264DLL_API int pauseVideos(int INSTANCE);
    PLAYH264DLL_API int playVideos(int INSTANCE);
    PLAYH264DLL_API int inputBuf(int INSTANCE, char *buf, int bufsize);
    PLAYH264DLL_API int resize(int INSTANCE, int width, int height);
    PLAYH264DLL_API int SetDrawLineCallBack(int INSTANCE, TDrawLineCallBack f1);
    PLAYH264DLL_API int SetBmpCallBack(int INSTANCE, TBmpCallBack bmp1);
    PLAYH264DLL_API int SetFillBmpCallBack(int INSTANCE, TDrawRectCallBack bmpf);
    PLAYH264DLL_API int SetH264CallBack(int INSTANCE, TH264CallBack yuvf);
    //support hardware acceleration, default is software decode.
    //hardware acceleration support h264 and YUV callback, software decode support all callback functions
    PLAYH264DLL_API int RevoHWAcceleration(int instance);

#ifdef __cplusplus
}
#endif // __cplusplus