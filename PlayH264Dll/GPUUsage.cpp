#include "stdafx.h"
#include "GPUUsage.h"
#include <nvapi.h>
#include <d3d9.h>
#include <vector>
#include <string>
using namespace std;

typedef struct container_adapter_to_physical_GPU
{
    UINT adapter;
    NvPhysicalGpuHandle physical_GPU_handle;
    NvU32 video_engine_load_percentage;
    bool usless_info;
}adapter_to_physical_GPU;

vector<adapter_to_physical_GPU*>            correspondence_list;
vector<adapter_to_physical_GPU*>::iterator  correspondence_list_iterator;
CRITICAL_SECTION critical_section_lock;

bool initial_NVIDIA_driver(void)
{
    if(NVAPI_OK != NvAPI_Initialize())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool free_NVIDIA_driver(void)
{
    if(NVAPI_OK != NvAPI_Unload())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool initial_usage_count(void)
{
    InitializeCriticalSection(&critical_section_lock);

    IDirect3D9* p_IDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION);
    if(NULL == p_IDirect3D9)
    {
        return false;
    }

    UINT adapter_count = p_IDirect3D9->GetAdapterCount();

    correspondence_list.resize(adapter_count);

    D3DADAPTER_IDENTIFIER9 d3d_adapter_identifier;
    NvDisplayHandle display_handle;
    NvPhysicalGpuHandle physical_GPU_handles[NVAPI_MAX_PHYSICAL_GPUS] = {0};
    NvU32 physical_GPU_count;
    adapter_to_physical_GPU* p_adapter_to_physical_GPU_temp;

    for(UINT i = 0; i < adapter_count; ++i)
    {
        correspondence_list[i] = new adapter_to_physical_GPU;
        correspondence_list[i]->adapter = 0;
        correspondence_list[i]->physical_GPU_handle = NULL;
        correspondence_list[i]->video_engine_load_percentage = 0;
        correspondence_list[i]->usless_info = false;

        correspondence_list[i]->adapter = i;

        if(D3D_OK != p_IDirect3D9->GetAdapterIdentifier(i, 0, &d3d_adapter_identifier))
        {
            correspondence_list[i]->usless_info = true;
            continue;
        }

        if(NVAPI_OK != NvAPI_GetAssociatedNvidiaDisplayHandle(d3d_adapter_identifier.DeviceName, &display_handle))
        {
            correspondence_list[i]->usless_info = true;
            continue;
        }

        if(NVAPI_OK != NvAPI_GetPhysicalGPUsFromDisplay(display_handle, physical_GPU_handles, &physical_GPU_count))
        {
            correspondence_list[i]->usless_info = true;
            continue;
        }

        if(0 != physical_GPU_count)
        {
            correspondence_list[i]->physical_GPU_handle = physical_GPU_handles[0];
        }

        if(1 < physical_GPU_count)
        {
            for(UINT x = 1; x < physical_GPU_count; ++x)
            {
                p_adapter_to_physical_GPU_temp = new adapter_to_physical_GPU;
                p_adapter_to_physical_GPU_temp->adapter = 0;
                p_adapter_to_physical_GPU_temp->physical_GPU_handle = NULL;
                p_adapter_to_physical_GPU_temp->video_engine_load_percentage = 0;
                p_adapter_to_physical_GPU_temp->usless_info = false;

                p_adapter_to_physical_GPU_temp->adapter = i;
                p_adapter_to_physical_GPU_temp->physical_GPU_handle = physical_GPU_handles[x];
                correspondence_list.push_back(p_adapter_to_physical_GPU_temp);
            }
        }
    }

    do
    {
        adapter_count = p_IDirect3D9->Release();
    } while(adapter_count > 0);

    vector<adapter_to_physical_GPU*>::iterator iterator_temp;

    for(correspondence_list_iterator = correspondence_list.begin(); correspondence_list_iterator != correspondence_list.end(); ++correspondence_list_iterator)
    {
        for(iterator_temp = correspondence_list_iterator;;)
        {
            ++iterator_temp;
            if(iterator_temp == correspondence_list.end())
            {
                break;
            }
            if((*correspondence_list_iterator)->physical_GPU_handle == (*iterator_temp)->physical_GPU_handle)
            {
                (*iterator_temp)->usless_info = true;
            }
        }
    }

    adapter_count = correspondence_list.size();

    for(UINT i = 0; i < adapter_count; i++)
    {
        for(correspondence_list_iterator = correspondence_list.begin(); correspondence_list_iterator != correspondence_list.end(); ++correspondence_list_iterator)
        {
            if((*correspondence_list_iterator)->usless_info)
            {
                delete (*correspondence_list_iterator);
                correspondence_list.erase(correspondence_list_iterator);
                break;
            }
        }
    }

    return true;
}