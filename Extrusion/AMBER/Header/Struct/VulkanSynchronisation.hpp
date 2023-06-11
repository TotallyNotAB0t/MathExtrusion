#ifndef __ENGINE_VULKAN_SYNCHRONISATION__
#define __ENGINE_VULKAN_SYNCHRONISATION__

#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"

struct VulkanSynchronisation
{
	std::vector<VkSemaphore> str_imageAvailableSemaphores;
	std::vector<VkSemaphore> str_renderFinishedSemaphores;
	std::vector<VkFence> str_inFlightFences;
	std::vector<VkFence> str_imagesInFlight;
	int MAX_FRAMES_IN_FLIGHT = 2;
};

#endif