#ifndef __ENGINE_VULKAN_DEVICE_MISC__
#define __ENGINE_VULKAN_DEVICE_MISC__

#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"

struct VulkanDeviceMisc
{
    GLFWwindow *str_window = nullptr;
    VkInstance str_instance = VK_NULL_HANDLE;
    VkQueue str_presentQueue = VK_NULL_HANDLE;
    VkSurfaceKHR str_surface = VK_NULL_HANDLE;
    VkQueue str_graphicsQueue = VK_NULL_HANDLE;
    VkDevice str_device = VK_NULL_HANDLE;
    VkPhysicalDevice str_physicalDevice = VK_NULL_HANDLE;
    VkSampleCountFlagBits str_msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    VkPhysicalDeviceProperties str_physicalDeviceProperties;
};

#endif //__ENGINE_VULKAN_DEVICE_MISC__