#pragma once
#include <Windows.h>

typedef int(*function_YUV420)(
    int instance,
    char* frame_buffer,
    int frame_buffer_size,
    int frame_width,
    int frame_height,
    size_t frame_ID,
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
