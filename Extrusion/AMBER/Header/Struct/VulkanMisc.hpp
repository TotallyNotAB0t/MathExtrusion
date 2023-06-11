#ifndef __ENGINE_VULKAN_MISC____
#define __ENGINE_VULKAN_MISC____

#include "VulkanDeviceMisc.hpp"
#include "VulkanSwapChainMisc.hpp"
#include "VulkanCommandeBufferMisc.hpp"
#include "VulkanDescriptor.hpp"
#include "VulkanSynchronisation.hpp"

struct VulkanMisc
{
    VulkanDeviceMisc *str_VulkanDeviceMisc;
    VulkanSwapChainMisc *str_VulkanSwapChainMisc;
    VulkanCommandeBufferMisc *str_VulkanCommandeBufferMisc;
    VulkanDescriptor *str_VulkanDescriptor;
	VulkanSynchronisation * str_VulkanSynchronisation;
};

#endif //__ENGINE_VULKAN_MISC____