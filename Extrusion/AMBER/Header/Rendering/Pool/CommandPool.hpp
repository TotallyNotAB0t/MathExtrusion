#ifndef __ENGINE_COMMAND_POOL__
#define __ENGINE_COMMAND_POOL__

#include "Debug.hpp"
#include <vector>
#include "VulkanMisc.hpp"

namespace Ge
{
	class CommandPool
	{
	private:
		friend class RenderingEngine;
		bool initialize(VulkanMisc * vM);
		void release();
	private:
		VkCommandPool m_commandPool;
		VulkanMisc * vulkanM;
	};
}

#endif //__ENGINE_COMMAND_POOL__