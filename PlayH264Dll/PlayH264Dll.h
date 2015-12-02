#pragma once
#ifdef PLAYH264DLL_EXPORTS
#define PLAYH264DLL_API __declspec(dllexport)
#else
#define PLAYH264DLL_API __declspec(dllimport)
#endif

typedef int(*function_YUV420)(int instance, char* frame_buffer, int frame_buffer_size, int frame_width, int frame_height, size_t frame_ID, void* userdata, int frame_lost); // YUV420 callback
typedef int(*function_NV12)(int instance, char* frame_buff, int frame_buffer_size, int frame_width, int frame_height, void* userdata, int frame_lost); // YV12 callback
typedef int(*function_H264)(int instance, char* frame_buff, int frame_buffer_size, int frame_width, int frame_height, void* userdata, int frame_lost); // h264 callback
typedef int(*function_RGB24)(int instance, char* frame_buff, int frame_buffer_size, int frame_width, int frame_height, void* userdata, int frame_lost); // RGB24 callback

typedef struct container_myparam_input
{
    int width;             // actual width
    int height;            // actual hight
    int playWidth;         // display width
    int playHeight;        // display hight
    int fps;               // frame per second
    HWND playHandle;       // handle to the paly window
    int stopPlay;          // play control
    int playChannle;
    bool isDecode;        // is decode
    //int  VideoCode;       //code type: 0: H264, 1: mpeg4
}myparamInput;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    //************************************
    // Function:  initial DLL and set max number of decodeing instance
    // Returns:   int: 0 success, -1 failure
    // Parameter: int max_number_of_decoding_instance
    //************************************
    PLAYH264DLL_API int initial_decode_DLL(int max_number_of_decoding_instance);

    //************************************
    // Function:  release DLL inner resource
    // Returns:   int: 0 success, -1 failure
    //************************************
    PLAYH264DLL_API int free_decode_DLL(void);

    //************************************
    // Function:  return idle instance
    // Returns:   int: idle instance number on success, -1 on failure
    // Parameter: void
    //************************************
    PLAYH264DLL_API int get_idle_decode_instance(void);

    //************************************
    // Function:  set decode parameters
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the idle instance return by get_idle_decode_instance()
    // Parameter: myparamInput* Myparam: decode parameters
    // Parameter: int type: encoding type, h264: 1 , mpeg: 2 ,PS: 3
    //************************************
    PLAYH264DLL_API int initial_decode_parameter(int instance, myparamInput* Myparam, int type);

    //************************************
    // Function:  decode a frame buffer
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the idle instance return by get_idle_decode_instance()
    // Parameter: unsigned char* pInBuffer: a frame's buffer
    // Parameter: int size: frame buffer size in bytes
    // Parameter: unsigned short sequence_number: for trace lost package,
    //            if you don't set trace_lost_package in callback functions to true,
    //            this parameter will be ignored, just pass a value into it.
    // Parameter: unsigned int timestamp: for trace lost package,
    //            if you don't set trace_lost_package in callback functions to true,
    //            this parameter will be ignored, just pass a value into it.
    //************************************
    PLAYH264DLL_API int decode(int instance, unsigned char* pInBuffer, int size, unsigned short sequence_number, unsigned int timestamp);

    //************************************
    // Function:  change inner GDI playing prarmeter to fit the window when the size changed;
    //            the HWND you passed in earlier corresponding to the window
    //            this only works when using software decode.
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the idle instance return by get_idle_decode_instance()
    //************************************
    PLAYH264DLL_API int playing_windows_RECT_changed_of_decode_DLL(int instance);

    //************************************
    // Function:  free decode instance specified by user
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the idle instance return by get_idle_decode_instance()
    //************************************
    PLAYH264DLL_API int free_decode_instance(int instance);

    //************************************
    // Function:  set YUV420 callback function,
    //            when this callback function set, the HWND you set won't dispaly.
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the instance you want to set callback function
    // Parameter: function_YUV420 p_function_YUV420: the function address
    // Parameter: void * additional_data: additional data, give back to you when the callback funciton is called
    // Parameter: bool trace_lost_package: true: trace lost package, will cause more calculations; false: don't trace lost package
    //************************************
    PLAYH264DLL_API int set_decode_YUV420_callback(int instance, function_YUV420 p_function_YUV420, void* additional_data, bool trace_lost_package);

    //************************************
    // Function:  set NV12 callback function
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the instance you want to set callback function
    // Parameter: function_NV12 p_function_NV12: the function address
    // Parameter: void * additional_data: additional data, give back to you when the callback funciton is called
    // Parameter: bool trace_lost_package: true: trace lost package, will cause more calculations; false: don't trace lost package
    //************************************
    PLAYH264DLL_API int set_decode_NV12_callback(int instance, function_NV12 p_function_NV12, void* additional_data, bool trace_lost_package);

    //************************************
    // Function:  set H264 callback function
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the instance you want to set callback function
    // Parameter: function_H264 p_function_H264: the function address
    // Parameter: void * additional_data: additional data, give back to you when the callback funciton is called
    // Parameter: bool trace_lost_package: true: trace lost package, will cause more calculations; false: don't trace lost package
    //************************************
    PLAYH264DLL_API int set_decode_H264_callback(int instance, function_H264 p_function_H264, void* additional_data, bool trace_lost_package);

    //************************************
    // Function:  set RGB24 callback function
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the instance you want to set callback function
    // Parameter: function_RGB24 p_function_RGB24: the function address
    // Parameter: void * additional_data: additional data, give back to you when the callback funciton is called
    // Parameter: bool trace_lost_package: true: trace lost package, will cause more calculations; false: don't trace lost package
    //************************************
    PLAYH264DLL_API int set_decode_RGB24_callback(int instance, function_RGB24 p_function_RGB24, void* additional_data, bool trace_lost_package);

    //************************************
    // Function:  set hardware acceleration, default is software decode
    //            hardware acceleration support h264 and YUV callback, software decode support all callback functions
    // Returns:   int: 0 success, -1 failure
    // Parameter: int instance: the instance you want to set whether use hardware acceleration
    // Parameter: bool acceleration: true: use hardware acceleration, false: use software decode
    //************************************
    PLAYH264DLL_API int set_decode_hardware_acceleration(int instance, bool acceleration = false);

    //************************************
    // the functions below is deprecated
    //************************************
    PLAYH264DLL_API int pauseVideos(int instance);

    PLAYH264DLL_API int playVideos(int instance);

    PLAYH264DLL_API int inputBuf(int instance, char *buf, int bufsize);

#ifdef __cplusplus
}
#endif // __cplusplus