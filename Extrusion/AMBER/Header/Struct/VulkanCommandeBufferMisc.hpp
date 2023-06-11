#ifndef __ENGINE_VULKAN_COMMANDE_BUFFER_MISC__
#define __ENGINE_VULKAN_COMMANDE_BUFFER_MISC__

#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"
#include <vector>

struct VulkanCommandeBufferMisc
{
    VkCommandPool str_commandPool = VK_NULL_HANDLE;
    std::vector<VkFramebuffer> str_swapChainFramebuffers;    
    std::vector<VkCommandBuffer> str_commandBuffers;
    VkImageView str_colorImageView = VK_NULL_HANDLE;
    VkImage str_colorImage = VK_NULL_HANDLE;
    VkDeviceMemory str_colorImageMemory = VK_NULL_HANDLE;
    VkImageView str_depthImageView = VK_NULL_HANDLE;
    VkImageView str_nullTexture = VK_NULL_HANDLE;
    VkSampler str_nullTextureSampler = VK_NULL_HANDLE;
    bool str_recreate = false;
};

#endif //__ENGINE_VULKAN_COMMANDE_BUFFER_MISC__