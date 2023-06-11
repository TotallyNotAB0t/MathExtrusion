#ifndef __ENGINE_LOGICAL_DEVICES__
#define __ENGINE_LOGICAL_DEVICES__

#include "PhysicalDevices.hpp"
#include "ValidationLayer.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanMisc.hpp"

namespace Ge
{
	class LogicalDevice
	{

	public:
		bool initialize(VulkanMisc * vM, WindowSurface * m_windowSurface);
		void release();
	private:
		VkQueue m_graphicsQueue;
		VkDevice m_device;
		VulkanMisc * vulkanM;
	};
}

#endif //__ENGINE_LOGICAL_DEVICES__