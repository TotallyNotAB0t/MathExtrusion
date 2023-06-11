#ifndef __ENGINE_COLOR_RESOURCES__
#define __ENGINE_COLOR_RESOURCES__

#include "Debug.hpp"
#include "VulkanMisc.hpp"

namespace Ge
{
	class ColorResources
	{
	private:
		friend class RenderingEngine;
		bool initialize(VulkanMisc * vM);
		void release();
	public:
		static bool createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VulkanMisc * vM);
		static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice pDevice);
	private:
		VulkanMisc *vulkanM;
		VkImageView m_colorImageView;
		VkImage m_colorImage;
		VkDeviceMemory m_colorImageMemory;
	};
}

#endif//__ENGINE_COLOR_RESOURCES__