#pragma once
#include <Windows.h>
//data struct for decode and play
typedef struct container_myparam_input
{
    int width;             //The actual width
    int height;            //The actual height
    int playWidth;         //Display width
    int playHeight;        //Display height
    int fps;               //frame per second
    HWND playHandle;       //window handle
    int stopPlay;          //play control
    int playChannle;       //play channle
    bool isDecode;
}myparamInput;

//call back functions begin

typedef int(*function_YUV420)(
    int instance,
    char* frame_buffer,
    int frame_buffer_size,
    int frame_width,
    int frame_height,
    int frame_ID,
    void* userdata,
    int frame_lost); // YUV420 callback

typedef int(*function_YV12)(
    int instance,
    char* frame_buff,
    int frame_buffer_size,
    int frame_width,
    int frame_height,
    void* userdata,
    int frame_lost); // YV12 callback

typedef int(*function_H264)(
    int instance,
    char* frame_buff,
    int frame_buffer_size,
    int frame_width,
    int frame_height,
    void* userdata,
    int frame_lost); // h264 callback

//call back functions end

//PlayH264DLL functions begin
typedef int(*function_initial_decode_DLL)(int max_number_of_decoding_instance);
typedef int(*function_free_decode_DLL)(void);
typedef int(*function_get_idle_instance)(void);
typedef int(*function_initial_decode_parameter)(int instance, myparamInput* Myparam, int type);
typedef int(*function_decode)(int instance, unsigned char* pInBuffer, int size);
typedef int(*function_free_decode_instance)(int instance);
typedef int(*function_set_YUV420_callback)(int instance, function_YUV420 p_function_YUV420, void* additional_data, bool trace_lost_package);
typedef int(*function_set_YV12_callback)(int instance, function_YV12 p_function_YV12, void* additional_data, bool trace_lost_package);
typedef int(*function_set_H264_callback)(int instance, function_H264 p_function_H264, void* additional_data, bool trace_lost_package);
typedef int(*function_set_hardware_acceleration)(int instance, bool acceleration);
typedef int(*function_pauseVideos)(int instance);
typedef int(*function_playVideos)(int instance);
typedef int(*function_inputBuf)(int instance, char *buf, int bufsize);
typedef int(*function_resize)(int instance, int width, int height);
//PlayH264DLL functions end


typedef struct
{
    UINT8 head;
    UINT8 PT;
    UINT8 length[2];
    UINT8 SSRC[4];
    UINT8 NTP[8];
    UINT8 RTP[4];
    UINT8 packetCount[4];
    UINT8 octetCount[4];
} RtcpSR;

typedef struct
{
    UINT8 head;
    UINT8 PT;
    UINT8 length[2];
    UINT8 SSRC[4];
    UINT8 SSRC_1[4];//SSRC of first source对应SR的SSRC
    UINT8 fractionLost;//从上个sr或rr以来的丢包率,表现为接收方此段时间内期待的RTP包与所收到的RTP包数目的差值和他期待的RTP报文的数目的比值，若为负值，置为0
    UINT8 cumulationLost[3];//The cumulative lost package
    UINT8 EHSNR[4];//extended highest sequence number received
    UINT8 interJitter[4];//到达时间抖动
    UINT8 LSR[4];//last SR
    UINT8 DLSR[4];//delay since last SR
} RtcpRR;

typedef struct
{
    UINT8 head;
    UINT8 PT;
    UINT8 length[2];
    UINT8 SSRC[4];
    UINT8 user[100]; // User description
}RtcpSDES;
// 这个按协议为扩张，但这里只取单一数据块情�?

typedef struct
{
    RtcpSR SR;
    RtcpSDES SDES;
}recieveSRFrom;

typedef struct
{
    RtcpRR RR;
    RtcpSDES SDES;
} sendRRTo;
