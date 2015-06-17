#include "stdafx.h"
#include "Direct3D9DisplayVideo.h"


CDirect3D9DisplayVideo::CDirect3D9DisplayVideo()
{
    m_p_IDirect3D9 = NULL;
    m_p_IDirect3DDevice9 = NULL;
    m_p_IDirect3DSurface9 = NULL;
    m_p_IDirect3DSurface9_back_buffer = NULL;

    m_p_D3DPRESENT_PARAMETERS = new D3DPRESENT_PARAMETERS;
    m_p_D3DLOCKED_RECT = new D3DLOCKED_RECT;
    m_p_CRITICAL_SECTION = new CRITICAL_SECTION;
}


CDirect3D9DisplayVideo::~CDirect3D9DisplayVideo()
{
    Cleanup();

    if(NULL != m_p_D3DPRESENT_PARAMETERS)
    {
        delete m_p_D3DPRESENT_PARAMETERS;
    }

    if(NULL != m_p_D3DLOCKED_RECT)
    {
        delete m_p_D3DLOCKED_RECT;
    }

    if(NULL != m_p_CRITICAL_SECTION)
    {
        delete m_p_CRITICAL_SECTION;
    }
}


BOOL CDirect3D9DisplayVideo::Initial(HWND hWnd, unsigned int off_screen_width, unsigned int off_screen_Height)
{
    GetClientRect(hWnd, &m_window_RECT);
    InitializeCriticalSection(m_p_CRITICAL_SECTION);

    Cleanup();

    m_p_IDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION);
    if(NULL == m_p_IDirect3D9)
    {
        return FALSE;
    }

    memset(m_p_D3DPRESENT_PARAMETERS, 0x0, sizeof(D3DPRESENT_PARAMETERS));
    m_p_D3DPRESENT_PARAMETERS->Windowed = TRUE;
    m_p_D3DPRESENT_PARAMETERS->SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_p_D3DPRESENT_PARAMETERS->BackBufferFormat = D3DFMT_UNKNOWN;

    m_h_result = m_p_IDirect3D9->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        m_p_D3DPRESENT_PARAMETERS,
        &m_p_IDirect3DDevice9);

    if(FAILED(m_h_result))
    {
        return FALSE;
    }

    m_h_result = m_p_IDirect3DDevice9->CreateOffscreenPlainSurface(
        off_screen_width,
        off_screen_Height,
        D3DFMT_R8G8B8,
        D3DPOOL_DEFAULT,
        &m_p_IDirect3DSurface9, NULL);

    if(FAILED(m_h_result))
    {
        return FALSE;
    }

    return TRUE;
}


BOOL CDirect3D9DisplayVideo::ReszieWindow(HWND hWnd)
{
    return GetClientRect(hWnd, &m_window_RECT);
}


BOOL CDirect3D9DisplayVideo::Render(BYTE* decoded_buffer_of_one_frame, int pixel_width, int pixel_Height)
{
    m_h_result = m_p_IDirect3DSurface9->LockRect(m_p_D3DLOCKED_RECT, NULL, D3DLOCK_DONOTWAIT);
    if(FAILED(m_h_result))
    {
        return FALSE;
    }

    byte *pSrc = decoded_buffer_of_one_frame;
    byte * pDest = (BYTE*)m_p_D3DLOCKED_RECT->pBits;
    int stride = m_p_D3DLOCKED_RECT->Pitch;
    unsigned long i = 0;

    int pixel_w_size = pixel_width * 3;
    for(i = 0; i < pixel_Height; i++)
    {
        memcpy(pDest, pSrc, pixel_w_size);
        pDest += stride;
        pSrc += pixel_w_size;
    }

    m_h_result = m_p_IDirect3DSurface9->UnlockRect();
    if(FAILED(m_h_result))
    {
        return FALSE;
    }

    m_h_result = m_p_IDirect3DDevice9->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    if(FAILED(m_h_result))
    {
        return FALSE;
    }

    m_h_result = m_p_IDirect3DDevice9->BeginScene();
    if(FAILED(m_h_result))
    {
        return FALSE;
    }

    m_h_result = m_p_IDirect3DDevice9->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_p_IDirect3DSurface9_back_buffer);
    if(FAILED(m_h_result))
    {
        return FALSE;
    }

    m_h_result = m_p_IDirect3DDevice9->StretchRect(m_p_IDirect3DSurface9, NULL, m_p_IDirect3DSurface9_back_buffer, &m_window_RECT, D3DTEXF_LINEAR);
    if(FAILED(m_h_result))
    {
        return FALSE;
    }

    m_h_result = m_p_IDirect3DDevice9->EndScene();
    if(FAILED(m_h_result))
    {
        return FALSE;
    }

    m_h_result = m_p_IDirect3DDevice9->Present(NULL, NULL, NULL, NULL);
    if(FAILED(m_h_result))
    {
        return FALSE;
    }

    return TRUE;
}


void CDirect3D9DisplayVideo::Cleanup(void)
{
    EnterCriticalSection(m_p_CRITICAL_SECTION);

    if(NULL != m_p_IDirect3DSurface9)
    {
        m_p_IDirect3DSurface9->Release();
    }

    if(NULL != m_p_IDirect3DDevice9)
    {
        m_p_IDirect3DDevice9->Release();
    }

    if(NULL != m_p_IDirect3D9)
    {
        m_p_IDirect3D9->Release();
    }

    LeaveCriticalSection(m_p_CRITICAL_SECTION);
}
