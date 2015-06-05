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
    // return value: 0 success, -1 failure
    RTSPFFMPEG_API int initial_RTSP_DLL(int max_number_of_playing_instance);


    // function    : uninitial RTSPFFmpeg.dll, release resource
    // return value: 0 success, -1 failure
    RTSPFFMPEG_API int free_RTSP_DLL(void);


    // function    : return idle instance
    // return value: idle instance number on success, -1 on failure
    RTSPFFMPEG_API int get_idle_instance(void);


    // function    : set IPC's URI, username and password
    // input       :
    //// INSTANCE  : the idle instance return by GetRtspINSTANCE
    //// URI       : IPC's URI(RTSP)
    //// userName  : IPC's username
    //// password  : IPC's password
    // return value: 0 success, -1 failure
    RTSPFFMPEG_API int initial_RTSP_parameter(int instance, char* URI, char* userName, char* password);


    // function    : connect to the IPC, you must have idle instance and initial IPC's video parameter
    // input       :
    //// INSTANCE  : the idle instance return by GetRtspINSTANCE
    // return value: 0 success, -1 failure
    RTSPFFMPEG_API int RTSP_connect(int instance);


    // function    : play realtime feedback of the IPC
    // input       :
    //// INSTANCE  : the idle instance return by GetRtspINSTANCE
    //// hWnd      : handle to the display window
    // return value: 0 success, -1 failure
    RTSPFFMPEG_API int play(int instance, HWND hWnd);


    // function    : pause realtime play
    // input       :
    //// INSTANCE  : the instance playing
    // return value: 0 success, -1 failure
    RTSPFFMPEG_API int pause(int instance);


    // function    : disconnect from IPC
    // input       : 
    //// INSTANCE  : the instance you get from GetRtspINSTANCE
    // return value: 0 success, -1 failure
    RTSPFFMPEG_API int RTSP_disconnect(int instance);


    // function    :set hardware acceleration, default is software decode
    RTSPFFMPEG_API int set_hardware_acceleration(int instance, bool acceleration = false);


    // function    : set YUV420 callback function
    // input       :
    //// instacne          : the instance you want to set callback function
    //// p_function_YUV420 : the function address
    //// buffer            : additional data, give back to you when the callback funciton is called
    //// trace_lost_package: true: trace lost package, will cause more calculations; false: don't trace lost package
    // return value: 0 success, -1 failure
    RTSPFFMPEG_API int set_YUV420_callback(int instance, function_YUV420 p_function_YUV420, void* additional_data, bool trace_lost_package);

    // function    :
    RTSPFFMPEG_API int set_YV12_callback(int instance, function_YV12 p_function_YV12, void* additional_data, bool trace_lost_package);

    RTSPFFMPEG_API int set_H264_callback(int instance, function_H264 p_function_H264, void* additional_data, bool trace_lost_package);

    RTSPFFMPEG_API int FreeRtspDLL0000(int instance);
    //RTSPFFMPEG_API int pSetDrawLineCallBack(int instance, TDrawLineCallBack f1);
    //RTSPFFMPEG_API int pSetPFCALLBACK(int instance, PFCALLBACK f1);
    //RTSPFFMPEG_API int pSetBmpCallBack(int instance, TBmpCallBack f2);
    //RTSPFFMPEG_API int pSetFillBmpCallBack(int instance, TDrawRectCallBack f3);
    //RTSPFFMPEG_API int pSetH264CallBack(int instance, TH264CallBack f3);

#ifdef __cplusplus
}
#endif // __cplusplus