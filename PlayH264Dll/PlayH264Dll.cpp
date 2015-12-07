// playH264ThreadDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "PlayH264Dll.h"
#include "DecodeH264threadClass.h"
#include "GPUUsage.h"
#pragma warning(disable: 4996)

using namespace std;

#ifdef _DEBUG // for memory leak check
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif // _DEBUG


typedef struct container_decode_data
{
    CDecode* p_CDecode;
    int idle; //1 enable 0 disable 2 lock
}decode_data;

decode_data* decode_list;
int          max_decode_number;

bool NVIDIA_GPU_driver_initialed;

int __decode(int instance, unsigned char* pInBuffer, int size, unsigned short sequence_number, unsigned int timestamp);

int check_instance(int instance)
{
    if(0 > instance || max_decode_number < instance || NULL == decode_list)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

PLAYH264DLL_API int initial_decode_DLL(int max_number_of_decoding_instance)
{
    if(0 >= max_number_of_decoding_instance)
    {
        return -1;
    }

    av_register_all();

    max_decode_number = max_number_of_decoding_instance;

    decode_list = new decode_data[max_decode_number];
    if(NULL == decode_list)
    {
        MessageBox(NULL, L"memory new error", NULL, MB_OK);
    }

    memset(decode_list, 0x0, max_decode_number * sizeof(decode_data));

    NVIDIA_GPU_driver_initialed = initial_NVIDIA_driver();

    if(NVIDIA_GPU_driver_initialed)
    {
        if(!initial_NVIDIA_GPU_usage_count())
        {
            return -1;
        }
    }

    return 0;
}

PLAYH264DLL_API int free_decode_DLL(void)
{
    for(int i = 0; i < max_decode_number; i++)
    {
        free_decode_instance(i);
    }

    delete[] decode_list;

    if(NVIDIA_GPU_driver_initialed)
    {
        free_NVIDIA_GPU_usage_count();
        free_NVIDIA_driver();
    }

    return 0;
}

PLAYH264DLL_API int get_idle_decode_instance(void)
{
    for(int i = 0; i < max_decode_number; ++i)
    {
        if(decode_list[i].idle == 0)
        {
            if(0 == check_instance(i))
            {
                decode_list[i].idle = 2;
                decode_list[i].p_CDecode = new CDecode;
                decode_list[i].p_CDecode->m_decode_instance = i;
                return i;
            }
        }
    }

    return -1;
}

DWORD WINAPI get_H264_out_of_PS(LPVOID lpParam)
{
    CDecode* p_CDecode = (CDecode*)lpParam;
    AVPacket* pAVPacket = (AVPacket*)av_malloc(sizeof(AVPacket));
    AVInputFormat* p_AVInputFormat = NULL;
    bool opened = false;
    int result = 0;

    Sleep(1000);

    while(p_CDecode->m_p_special_context_for_PS->stream_opened)
    {
        Sleep(20);
        if(!opened)
        {
            result = av_probe_input_buffer2(
                p_CDecode->m_p_special_context_for_PS->p_AVIOContext,
                &p_AVInputFormat,
                "",
                NULL,
                0,
                0);

            result = avformat_open_input(
                &p_CDecode->m_p_special_context_for_PS->p_AVFormatContext,
                "",
                p_AVInputFormat,
                NULL);
            if(0 != result)
            {
                result = avformat_open_input(
                    &p_CDecode->m_p_special_context_for_PS->p_AVFormatContext,
                    "",
                    NULL,
                    NULL);
            }

            result = avformat_find_stream_info(
                p_CDecode->m_p_special_context_for_PS->p_AVFormatContext,
                NULL);
            if(0 > result)
            {
                MessageBoxA(NULL, "avformat_find_stream_info error", "ERROR", MB_OK);
                return -1;
            }

            for(p_CDecode->m_p_special_context_for_PS->video_stream_index = 0;
                p_CDecode->m_p_special_context_for_PS->video_stream_index < p_CDecode->m_p_special_context_for_PS->p_AVFormatContext->nb_streams;
                p_CDecode->m_p_special_context_for_PS->video_stream_index++)
            {
                if(AVMEDIA_TYPE_VIDEO == p_CDecode->m_p_special_context_for_PS->p_AVFormatContext->streams[p_CDecode->m_p_special_context_for_PS->video_stream_index]->codec->codec_type)
                {
                    p_CDecode->m_p_special_context_for_PS->CodecID = p_CDecode->m_p_special_context_for_PS->p_AVFormatContext->streams[p_CDecode->m_p_special_context_for_PS->video_stream_index]->codec->codec_id;
                    break;
                }
            }

            p_CDecode->bmpinfo.bmiHeader.biHeight = p_CDecode->m_p_special_context_for_PS->p_AVFormatContext->streams[p_CDecode->m_p_special_context_for_PS->video_stream_index]->codec->height;
            p_CDecode->bmpinfo.bmiHeader.biWidth = p_CDecode->m_p_special_context_for_PS->p_AVFormatContext->streams[p_CDecode->m_p_special_context_for_PS->video_stream_index]->codec->width;
            p_CDecode->paramUser.height = p_CDecode->bmpinfo.bmiHeader.biHeight;
            p_CDecode->paramUser.width = p_CDecode->bmpinfo.bmiHeader.biWidth;

            opened = true;
        }


        av_read_frame(
            p_CDecode->m_p_special_context_for_PS->p_AVFormatContext,
            pAVPacket);
        if(pAVPacket->stream_index == p_CDecode->m_p_special_context_for_PS->video_stream_index)
        {
            result = __decode(p_CDecode->m_decode_instance, pAVPacket->data, pAVPacket->size, 0, 0);
        }
        av_free_packet(pAVPacket);
    }

    return 0;
}

PLAYH264DLL_API int initial_decode_parameter(int instance, myparamInput* Myparam, int type)
{
    if(NULL == Myparam || 0 > check_instance(instance))
    {
        return -1;
    }

    decode_list[instance].p_CDecode->type = type;

    AVCodecID codeType = AV_CODEC_ID_NONE;
    int result = 0;
    DWORD temp;

    switch(decode_list[instance].p_CDecode->type)
    {
        case 1:
            codeType = AV_CODEC_ID_H264;
            break;
        case 2:
            codeType = AV_CODEC_ID_MPEG4;
            break;
        default:
            break;
    }

    if(3 != type)
    {
        decode_list[instance].p_CDecode->m_p_AVCodecParserContext = av_parser_init(codeType);
        decode_list[instance].p_CDecode->m_p_AVCodec = avcodec_find_decoder(codeType);
        decode_list[instance].p_CDecode->m_p_AVCodecContext = avcodec_alloc_context3(decode_list[instance].p_CDecode->m_p_AVCodec);

        decode_list[instance].idle = 1;

        return decode_list[instance].p_CDecode->InputParam(Myparam);
    }
    else
    {
        decode_list[instance].p_CDecode->m_p_AVCodecParserContext = av_parser_init(AV_CODEC_ID_H264);
        decode_list[instance].p_CDecode->m_p_AVCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
        decode_list[instance].p_CDecode->m_p_AVCodecContext = avcodec_alloc_context3(decode_list[instance].p_CDecode->m_p_AVCodec);

        decode_list[instance].idle = 1;

        result = decode_list[instance].p_CDecode->InputParam(Myparam);
        result = decode_list[instance].p_CDecode->initial_PS_context();

        decode_list[instance].p_CDecode->m_p_special_context_for_PS->thread_handle = CreateThread(NULL, 0, get_H264_out_of_PS, decode_list[instance].p_CDecode, 0, &temp);

        return result;
    }
}

int __decode(int instance, unsigned char* pInBuffer, int size, unsigned short sequence_number, unsigned int timestamp)
{
    int pos = 0;
    int64_t pts = AV_NOPTS_VALUE;
    int64_t dts = AV_NOPTS_VALUE;
    uint8_t *pout;// a complete frame, if incomplete it's NULL
    int pout_len;// a complete frame's length, if incomplete it's 0
    int len;

    // if trace lost package
    if(decode_list[instance].p_CDecode->m_trace_lost_package)
    {
        // when it is a new frame
        if(timestamp != decode_list[instance].p_CDecode->m_previous_timestamp)
        {
            // save previous frame ID and number of lost package
            decode_list[instance].p_CDecode->m_previous_frame_ID = decode_list[instance].p_CDecode->m_frame_ID;
            decode_list[instance].p_CDecode->m_previous_number_of_lost_package = decode_list[instance].p_CDecode->m_number_of_lost_package;
            // great change in sequence number, lost frame, not packages
            if(sequence_number - decode_list[instance].p_CDecode->m_previous_sequence_number > 20)
            {
                decode_list[instance].p_CDecode->m_frame_ID += ((sequence_number - decode_list[instance].p_CDecode->m_previous_sequence_number) / 6);
            }
            // frame ID plus 1
            else
            {
                ++(decode_list[instance].p_CDecode->m_frame_ID);
            }
            // set number of lost package back to 0
            decode_list[instance].p_CDecode->m_number_of_lost_package = 0;
            // set timestamp and sequence number for new check round
            decode_list[instance].p_CDecode->m_previous_timestamp = timestamp;
            decode_list[instance].p_CDecode->m_previous_sequence_number = sequence_number;
        }
        // the same frame
        else
        {
            // if there is lost package, calculate how many lost
            decode_list[instance].p_CDecode->m_number_of_lost_package += (sequence_number - decode_list[instance].p_CDecode->m_previous_sequence_number - 1);
            decode_list[instance].p_CDecode->m_previous_sequence_number = sequence_number;
        }
    }

    do
    {
        len = av_parser_parse2(decode_list[instance].p_CDecode->m_p_AVCodecParserContext, decode_list[instance].p_CDecode->m_p_AVCodecContext, &pout, &pout_len, pInBuffer + pos, size - pos, pts, dts, AV_NOPTS_VALUE);

        pos += len;

        if(pout_len > 0)
        {
            if(decode_list[instance].idle == 1)
            {
                decode_list[instance].p_CDecode->writeNetBuf(instance, pout, pout_len);
            }
        }
    } while(pos < size);

    return 0;
}

#include "PSData.h"

PLAYH264DLL_API int decode(int instance, unsigned char* pInBuffer, int size, unsigned short sequence_number, unsigned int timestamp)
{
    if(0 > check_instance(instance) || 1 != decode_list[instance].idle || NULL == decode_list[instance].p_CDecode->m_p_AVCodecParserContext)
    {
        return -1;
    }

    PS_data* buffer = NULL;

    if(3 != decode_list[instance].p_CDecode->type)
    {
        return __decode(instance, pInBuffer, size, sequence_number, timestamp);
    }
    else
    {
        buffer = new PS_data;
        if(NULL != buffer)
        {
            buffer->data = new uint8_t[size];
            if(NULL != buffer->data)
            {
                if(0 < size)
                {
                    memcpy(buffer->data, pInBuffer, size);
                    buffer->size = size;
                    concurrent_queue_pushback(
                        decode_list[instance].p_CDecode->m_p_special_context_for_PS->PS_data_queue,
                        buffer);
                }
                else
                {
                    delete buffer->data;
                    delete buffer;
                }
            }
        }

        return 0;
    }
}

PLAYH264DLL_API int playing_windows_RECT_changed_of_decode_DLL(int instance)
{
    if(0 > check_instance(instance))
    {
        return -1;
    }

    return  decode_list[instance].p_CDecode->playResize();
}

PLAYH264DLL_API int free_decode_instance(int instance)
{
    if(0 > check_instance(instance) || 1 != decode_list[instance].idle)
    {
        return -1;
    }

    PS_data* buffer = NULL;

    if(3 == decode_list[instance].p_CDecode->type)
    {
        decode_list[instance].p_CDecode->m_p_special_context_for_PS->stream_opened = false;
        WaitForSingleObject(decode_list[instance].p_CDecode->m_p_special_context_for_PS->thread_handle, INFINITE);
        CloseHandle(decode_list[instance].p_CDecode->m_p_special_context_for_PS->thread_handle);
        decode_list[instance].p_CDecode->m_p_special_context_for_PS->thread_handle = INVALID_HANDLE_VALUE;

        do
        {
            buffer = (PS_data*)concurrent_queue_pophead(decode_list[instance].p_CDecode->m_p_special_context_for_PS->PS_data_queue);
            if(NULL != buffer)
            {
                free(buffer->data);
                free(buffer);
            }
        } while(NULL != buffer);
    }

    decode_list[instance].idle = 2;//ensure locks
    decode_list[instance].p_CDecode->freeParam();

    DWORD exit_code = STILL_ACTIVE;
    while(STILL_ACTIVE == exit_code)
    {
        Sleep(200);
        GetExitCodeThread(decode_list[instance].p_CDecode->hThreadDecode, &exit_code);
    }

    CloseHandle(decode_list[instance].p_CDecode->hThreadDecode);

    decode_list[instance].p_CDecode->dataQueueClean();

    av_parser_close(decode_list[instance].p_CDecode->m_p_AVCodecParserContext);
    avcodec_close(decode_list[instance].p_CDecode->m_p_AVCodecContext);
    av_freep(decode_list[instance].p_CDecode->m_p_AVCodecContext);

    if(NULL != decode_list[instance].p_CDecode)
    {
        delete decode_list[instance].p_CDecode;
    }

    decode_list[instance].idle = 0;// release lock

    return 0;
}

PLAYH264DLL_API int set_decode_YUV420_callback(int instance, function_YUV420 p_function_YUV420, void* additional_data, bool trace_lost_package)
{
#ifdef _DEBUG
    if(0 > check_instance(instance))
#else
    if(0 > check_instance(instance) || NULL == p_function_YUV420)
#endif
    {
        return -1;
    }

    decode_list[instance].p_CDecode->m_p_function_YUV420 = p_function_YUV420;
    decode_list[instance].p_CDecode->m_p_YUV420_extra_data = additional_data;
    decode_list[instance].p_CDecode->m_trace_lost_package = trace_lost_package;

    return 0;
}

PLAYH264DLL_API int set_decode_NV12_callback(int instance, function_NV12 p_function_NV12, void* additional_data, bool trace_lost_package)
{
    if(NULL == p_function_NV12 || 0 > check_instance(instance))
    {
        return -1;
    }

    decode_list[instance].p_CDecode->m_p_function_NV12 = p_function_NV12;
    decode_list[instance].p_CDecode->m_p_NV12_extra_data = additional_data;
    decode_list[instance].p_CDecode->m_trace_lost_package = trace_lost_package;

    return 0;
}

PLAYH264DLL_API int set_decode_H264_callback(int instance, function_H264 p_function_H264, void* additional_data, bool trace_lost_package)
{
    if(NULL == p_function_H264 || 0 > check_instance(instance))
    {
        return -1;
    }

    decode_list[instance].p_CDecode->m_p_function_H264 = p_function_H264;
    decode_list[instance].p_CDecode->m_p_H264_extra_data = additional_data;
    decode_list[instance].p_CDecode->m_trace_lost_package = trace_lost_package;

    return 0;
}

PLAYH264DLL_API int set_decode_RGB24_callback(int instance, function_RGB24 p_function_RGB24, void* additional_data, bool trace_lost_package)
{
    if(NULL == p_function_RGB24 || 0 > check_instance(instance))
    {
        return -1;
    }

    decode_list[instance].p_CDecode->m_p_function_RGB24 = p_function_RGB24;
    decode_list[instance].p_CDecode->m_p_RGB24_extra_data = additional_data;
    decode_list[instance].p_CDecode->m_trace_lost_package = trace_lost_package;

    return 0;
}

//support hardware acceleration, default is software decode.
//hardware acceleration support h264 and YUV callback, software decode support all callback functions
PLAYH264DLL_API int set_decode_hardware_acceleration(int instance, bool acceleration)
{
    if(0 > check_instance(instance) || NULL == decode_list[instance].p_CDecode)
    {
        return -1;
    }

    if(NVIDIA_GPU_driver_initialed)
    {
        decode_list[instance].p_CDecode->m_b_hardware_acceleration = acceleration;
    }

    return 0;
}

PLAYH264DLL_API int pauseVideos(int instance)
{
    if(0 > check_instance(instance))
    {
        return -1;
    }
    decode_list[instance].p_CDecode->pauseVideo();
    return 0;
}

PLAYH264DLL_API int playVideos(int instance)
{
    if(0 > check_instance(instance))
    {
        return -1;
    }
    decode_list[instance].p_CDecode->playVideo();
    return 0;
}

PLAYH264DLL_API int inputBuf(int instance, char *buf, int bufsize)
{
    if(0 > check_instance(instance) || 1 != decode_list[instance].idle || NULL == buf || 0 > bufsize)
    {
        return -1;
    }
    decode_list[instance].p_CDecode->writeNetBuf(instance, (unsigned char*)buf, bufsize);
    return 0;
}