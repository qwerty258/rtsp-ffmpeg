#pragma once
#ifdef RTSPFFMPEG_EXPORTS
#define RTSPFFMPEG_API __declspec(dllexport)
#else
#define RTSPFFMPEG_API __declspec(dllimport)
#endif

#include <Windows.h>

typedef int(*function_YUV420)(int instance, char* frame_buffer, int frame_buffer_size, int frame_width, int frame_height, size_t frame_ID, void* userdata, int frame_lost); // YUV420 callback
typedef int(*function_YV12)(int instance, char* frame_buff, int frame_buffer_size, int frame_width, int frame_height, void* userdata, int frame_lost); // YV12 callback
typedef int(*function_H264)(int instance, char* frame_buff, int frame_buffer_size, int frame_width, int frame_height, void* userdata, int frame_lost); // h264 callback

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    //************************************
    // Function:  initial RTSPFFmpeg.dll
    // Returns:   int: 0 success, -1 failure
    // Parameter: int max_number_of_playing_instance
    //************************************
    RTSPFFMPEG_API int initial_RTSP_DLL(int max_number_of_playing_instance);


    //************************************
    // Function:  uninitial RTSPFFmpeg.dll, release resource
    // Returns:   int: 0 success, -1 failure
    // Parameter: void
    //************************************
    RTSPFFMPEG_API int free_RTSP_DLL(void);


    //************************************
    // Function:  return idle instance
    // Returns:   int: idle instance number on success, -1 on failure
    // Parameter: void
    //************************************
    RTSPFFMPEG_API int get_idle_instance(void);


    //************************************
    // Function:  set IPC's URI, username and password
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the idle instance return by get_idle_instance()
    // Parameter: char * URI: IPC's URI(RTSP)
    // Parameter: char * userName: IPC's username
    // Parameter: char * password: IPC's password
    // Parameter: HWND hWnd: handle of the window to display video
    //************************************
    RTSPFFMPEG_API int initial_RTSP_parameter(int instance, char* URI, char* userName, char* password, HWND hWnd);


    //************************************
    // Function:  free RTSP instance specified by user
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance
    //************************************
    RTSPFFMPEG_API int free_RTSP_instance(int instance);


    //************************************
    // Function:  connect to the IPC, you must have idle instance and initial IPC's video parameter
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the idle instance return by get_idle_instance()
    //************************************
    RTSPFFMPEG_API int RTSP_connect(int instance);


    //************************************
    // Function:  play feedback of the IPC in realtime
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the idle instance return by get_idle_instance()
    //************************************
    RTSPFFMPEG_API int play(int instance);


    //************************************
    // Function:  pause realtime play
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the instance playing
    //************************************
    RTSPFFMPEG_API int pause(int instance);


    //************************************
    // Function:  disconnect from IPC
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the instance you get from get_idle_instance()
    //************************************
    RTSPFFMPEG_API int RTSP_disconnect(int instance);


    //************************************
    // Function:  set hardware acceleration, default is software decode
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the instance you want to set whether use hardware acceleration
    // Parameter: bool acceleration: true: use hardware acceleration, false: use software decode
    //************************************
    RTSPFFMPEG_API int set_hardware_acceleration(int instance, bool acceleration = false);


    //************************************
    // Function:  set YUV420 callback function
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the instance you want to set callback function
    // Parameter: function_YUV420 p_function_YUV420: the function address
    // Parameter: void * additional_data: additional data, give back to you when the callback funciton is called
    // Parameter: bool trace_lost_package: true: trace lost package, will cause more calculations; false: don't trace lost package
    //************************************
    RTSPFFMPEG_API int set_YUV420_callback(int instance, function_YUV420 p_function_YUV420, void* additional_data, bool trace_lost_package);

    //************************************
    // Function:  set YV12 callback function
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the instance you want to set callback function
    // Parameter: function_YV12 p_function_YV12: the function address
    // Parameter: void * additional_data: additional data, give back to you when the callback funciton is called
    // Parameter: bool trace_lost_package: true: trace lost package, will cause more calculations; false: don't trace lost package
    //************************************
    RTSPFFMPEG_API int set_YV12_callback(int instance, function_YV12 p_function_YV12, void* additional_data, bool trace_lost_package);

    //************************************
    // Function:  set H264 callback function
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the instance you want to set callback function
    // Parameter: function_H264 p_function_H264: the function address
    // Parameter: void * additional_data: additional data, give back to you when the callback funciton is called
    // Parameter: bool trace_lost_package: true: trace lost package, will cause more calculations; false: don't trace lost package
    //************************************
    RTSPFFMPEG_API int set_H264_callback(int instance, function_H264 p_function_H264, void* additional_data, bool trace_lost_package);

#ifdef __cplusplus
}
#endif // __cplusplus