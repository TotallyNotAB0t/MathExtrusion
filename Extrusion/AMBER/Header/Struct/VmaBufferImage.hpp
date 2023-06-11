#ifndef __ENGINE_VMA_BUFFER_IMAGE__
#define __ENGINE_VMA_BUFFER_IMAGE__

using namespace std;
#include <iostream>
#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"

struct VmaBufferImage
{
    VkImage image;
    VmaAllocation allocation;
};

#endif //__ENGINE_VMA_BUFFER_IMAGE__