#pragma once
#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")

class CDirect3D9DisplayVideo
{
private:
    // interface pointers
    IDirect3D9*             m_p_IDirect3D9;
    IDirect3DDevice9*       m_p_IDirect3DDevice9;
    IDirect3DSurface9*      m_p_IDirect3DSurface9;
    IDirect3DSurface9*      m_p_IDirect3DSurface9_back_buffer;
    // data structures
    RECT                    m_window_RECT;
    HRESULT                 m_h_result;
    D3DPRESENT_PARAMETERS*  m_p_D3DPRESENT_PARAMETERS;
    D3DLOCKED_RECT*         m_p_D3DLOCKED_RECT;
    CRITICAL_SECTION*       m_p_CRITICAL_SECTION;
public:
    CDirect3D9DisplayVideo();
    ~CDirect3D9DisplayVideo();
    BOOL Initial(HWND hWnd, unsigned int lWidth, unsigned int lHeight);
    BOOL ReszieWindow(HWND hWnd);
    BOOL Render(BYTE* decoded_buffer_of_one_frame, int pixel_width, int pixel_Height);
    void Cleanup(void);
};

