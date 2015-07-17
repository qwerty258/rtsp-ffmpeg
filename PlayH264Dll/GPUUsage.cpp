#include "stdafx.h"
#include "GPUUsage.h"
#include <nvapi.h>

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