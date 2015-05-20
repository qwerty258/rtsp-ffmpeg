/*****************************************************************************
 * va.h: Video Acceleration API for avcodec
 *****************************************************************************
 * Copyright (C) 2012 tuyuandong
 *
 * Authors: Yuandong Tu <tuyuandong@gmail.com>
 *
 * This file is part of FFmpeg.
 *****************************************************************************/

#ifndef _TYD_VA_H
#define _TYD_VA_H 1

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec\avcodec.h>
#include <libavutil\pixfmt.h>
#include <libavutil\pixdesc.h>
#ifdef __cplusplus
}
#endif

struct va_sys_t;
typedef struct dxva_t dxva_t;
typedef struct va_sys_t va_dxva2_t;

struct dxva_t
{
    va_dxva2_t *sys;
    int		pix_fmt;
    int(*setup)(va_dxva2_t *va, void **hw, const AVCodecContext *avctx);
    int(*get)(va_dxva2_t *, AVFrame *frame);
    void(*release)(va_dxva2_t *, AVFrame *frame);
    int(*extract)(va_dxva2_t *, AVFrame *src, AVFrame *dst, void *);
};

static inline int dxva_Setup(dxva_t *va, void **hw, const AVCodecContext *avctx)
{
    return va->setup(va->sys, hw, avctx);
}
static inline int dxva_Get(dxva_t *va, AVFrame *frame)
{
    return va->get(va->sys, frame);
}
static inline void dxva_Release(dxva_t *va, AVFrame *frame)
{
    va->release(va->sys, frame);
}

static inline int dxva_Extract(dxva_t *va, AVFrame *src, AVFrame *dst, void *p)
{
    return va->extract(va->sys, src, dst, p);
}

extern dxva_t *dxva_New(int);

extern void dxva_Delete(dxva_t *va);


#if 1

enum PixelFormat DxGetFormat(AVCodecContext *avctx, const enum PixelFormat *pi_fmt);

int DxGetFrameBuf(struct AVCodecContext *avctx, AVFrame *pic);

int	DxReGetFrameBuf(struct AVCodecContext *avctx, AVFrame *pic);

void DxReleaseFrameBuf(struct AVCodecContext *avctx, AVFrame *pic);

int DxPictureCopy(struct AVCodecContext *avctx, AVFrame *src, AVFrame* dst, void*);
#endif


#endif //_TYD_VA_H