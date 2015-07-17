#pragma once

bool initial_NVIDIA_driver(void);

bool free_NVIDIA_driver(void);

bool initial_NVIDIA_GPU_usage_count(void);

bool free_NVIDIA_GPU_usage_count(void);

bool get_NVIDIA_GPU_usage(void);

bool is_NVIDIA_GPU_usage_full(void);

unsigned int get_most_idle_NVIDIA_GPU(void);