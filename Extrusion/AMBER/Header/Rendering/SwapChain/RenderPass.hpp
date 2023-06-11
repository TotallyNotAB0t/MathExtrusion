#ifndef __ENGINE_RENDER_PASS__
#define __ENGINE_RENDER_PASS__

#include "Debug.hpp"
#include <array>
#include "VulkanMisc.hpp"

namespace Ge
{
	class RenderPass
	{
	private:
		friend class SwapChain;
		bool initialize(VkFormat swapChainImageFormat, VkSampleCountFlagBits msaaSamples, VulkanMisc * vM);
		void release();
	public:
		static VkFormat findDepthFormat(VulkanMisc * vM);
		static VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VulkanMisc * vM);
	private:
		VkRenderPass m_renderPass;
		VkRenderPass m_renderPassSkybox;
		VulkanMisc * vulkanM;
	};
}

#endif//__ENGINE_RENDER_PASS__