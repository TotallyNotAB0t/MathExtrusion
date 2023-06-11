#ifndef __ENGINE_SYNC_OBJECTS__
#define __ENGINE_SYNC_OBJECTS__

#include "Debug.hpp"
#include <vector>
#include "VulkanMisc.hpp"

namespace Ge
{
	class SyncObjects
	{
	private:
		friend class RenderingEngine;
		bool initialize(VulkanMisc * vM);
		void release();
	private:
		VulkanMisc * vulkanM;
		std::vector<VkSemaphore> m_imageAvailableSemaphores;
		std::vector<VkSemaphore> m_renderFinishedSemaphores;
		std::vector<VkFence> m_inFlightFences;
		std::vector<VkFence> m_imagesInFlight;
		const int MAX_FRAMES_IN_FLIGHT = 2;
	};
}

#endif //__ENGINE_SYNC_OBJECTS__