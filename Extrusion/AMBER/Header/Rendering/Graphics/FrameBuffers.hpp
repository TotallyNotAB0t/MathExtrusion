#ifndef __ENGINE_FRAME_BUFFERS__
#define __ENGINE_FRAME_BUFFERS__

#include "Debug.hpp"
#include <array>
#include "VulkanMisc.hpp"

namespace Ge
{
	class FrameBuffers
	{
	private:
		friend class RenderingEngine;
		bool initialize(VulkanMisc * vM);
		void release();
	private:
		VulkanMisc * vulkanM;
		std::vector<VkFramebuffer> m_swapChainFramebuffers;
	};
}

#endif // __ENGINE_FRAME_BUFFERS__
