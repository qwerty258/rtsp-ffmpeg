/*****************************************************************************
 * va.h: Video Acceleration API for avcodec
 *****************************************************************************
 * Copyright (C) 2012 tuyuandong
 *
 * Authors: Yuandong Tu <tuyuandong@gmail.com>
 *
 * This file is part of FFmpeg.
 *****************************************************************************/
#include "stdafx.h"
#include "windows.h"
#include "va.h"

#ifdef _DEBUG // for memory leak check
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif // _DEBUG

//#define log_va
extern HWND gPlayWnd;
enum PixelFormat DxGetFormat(AVCodecContext *avctx,
    const enum PixelFormat *pi_fmt)
{
#ifdef log_va
    char a[15] = "c:\\";
    itoa((int)gPlayWnd,a+3,10);
    FILE *fp = fopen(a,"a+");
    fputs("进入DXGetFormat：\n",fp);
    fclose(fp);
#endif 

    unsigned int i;
    dxva_t *p_va = (dxva_t*)avctx->opaque;

    if(p_va != NULL)
        dxva_Delete(p_va);

    p_va = dxva_New(avctx->codec_id);
    if(p_va != NULL)
    {
        /* Try too look for a supported hw acceleration */
        for(i = 0; pi_fmt[i] != PIX_FMT_NONE; i++)
        {
            const char *name = av_get_pix_fmt_name(pi_fmt[i]);
            av_log(NULL, AV_LOG_DEBUG, "Available decoder output format %d (%s)", pi_fmt[i], name ? name : "unknown");
            if(p_va->pix_fmt != pi_fmt[i])
                continue;

            /* We try to call dxva_Setup when possible to detect errors when
             * possible (later is too late) */
            if(avctx->width > 0 && avctx->height > 0 && dxva_Setup(p_va, &avctx->hwaccel_context, avctx))
            {
                av_log(NULL, AV_LOG_ERROR, "acceleration setup failure");
#ifdef log_va
                fp = fopen(a,"a+");
                fputs("退出DXGetFormat：acceleration setup failure\n",fp);
                char b[10];

                fclose(fp);
#endif 
                break;
            }

            //if( p_va->description )
            //    av_log(NULL,AV_LOG_INFO, "Using %s for hardware decoding.",
            //              p_va->description );

            /* FIXME this will disable direct rendering
             * even if a new pixel format is renegotiated
             */
            //p_sys->b_direct_rendering = false;
            //p_sys->p_va = p_va;
            avctx->opaque = p_va;
            avctx->draw_horiz_band = NULL;
#ifdef log_va
            fp = fopen(a,"a+");
            fputs("退出DXGetFormat：成功\n",fp);
            fclose(fp);
#endif 
            return pi_fmt[i];

        }

        av_log(NULL, AV_LOG_ERROR, "acceleration not available");
        dxva_Delete(p_va);
#ifdef log_va
        fp = fopen(a,"a+");
        fputs("退出DXGetFormat：不支持硬件加速\n",fp);
        fclose(fp);
#endif 
    }
#ifdef log_va
    fp = fopen(a,"a+");
    fputs("退出DXGetFormat：va申请失败\n",fp);
    fclose(fp);
#endif 
    avctx->opaque = NULL;
    /* Fallback to default behaviour */
    return avcodec_default_get_format(avctx, pi_fmt);
}


/*****************************************************************************
 * DxGetFrameBuf: callback used by ffmpeg to get a frame buffer.
 *****************************************************************************
 * It is used for direct rendering as well as to get the right PTS for each
 * decoded picture (even in indirect rendering mode).
 *****************************************************************************/
int DxGetFrameBuf(struct AVCodecContext *avctx, AVFrame *pic)
{
    dxva_t *p_va = (dxva_t *)avctx->opaque;
    //picture_t *p_pic;

    /* */
    pic->reordered_opaque = avctx->reordered_opaque;
    pic->opaque = NULL;

    if(p_va)
    {
        /* hwaccel_context is not present in old ffmpeg version */
        if(dxva_Setup(p_va, &avctx->hwaccel_context, avctx))
        {
            av_log(NULL, AV_LOG_ERROR, "vlc_va_Setup failed");
            return -1;
        }

        /* */
        pic->type = FF_BUFFER_TYPE_USER;

#if LIBAVCODEC_VERSION_MAJOR < 54
        pic->age = 256*256*256*64;
#endif

        if(dxva_Get(p_va, pic))
        {
            av_log(NULL, AV_LOG_ERROR, "VaGrabSurface failed");
            return -1;
        }
        return 0;
    }
    return avcodec_default_get_buffer(avctx, pic);
}
int  DxReGetFrameBuf(struct AVCodecContext *avctx, AVFrame *pic)
{

    pic->reordered_opaque = avctx->reordered_opaque;

    /* We always use default reget function, it works perfectly fine */
    return avcodec_default_reget_buffer(avctx, pic);
}

void DxReleaseFrameBuf(struct AVCodecContext *avctx, AVFrame *pic)
{
    dxva_t *p_va = (dxva_t *)avctx->opaque;
    int i;

    if(p_va)
    {
        dxva_Release(p_va, pic);
    }
    else if(!pic->opaque)
    {
        /* We can end up here without the AVFrame being allocated by
         * avcodec_default_get_buffer() if VA is used and the frame is
         * released when the decoder is closed
         */
        if(pic->type == FF_BUFFER_TYPE_INTERNAL)
            avcodec_default_release_buffer(avctx, pic);
    }
    else
    {
        //picture_t *p_pic = (picture_t*)p_ff_pic->opaque;
        //decoder_UnlinkPicture( p_dec, p_pic );
        av_log(NULL, AV_LOG_ERROR, "%d %s a error is rasied\r\n");
    }
    for(i = 0; i < 4; i++)
        pic->data[i] = NULL;

}

int DxPictureCopy(struct AVCodecContext *avctx, AVFrame *src, AVFrame* dst, void * YUVFunc)
{
    dxva_t *p_va = (dxva_t *)avctx->opaque;
    return dxva_Extract(p_va, src, dst, YUVFunc);
}