#ifndef __ENGINE_TEXTURES__
#define __ENGINE_TEXTURES__

#include "stb-cmake/stb_image.h"
#include "VulkanMisc.hpp"
#include "BufferManager.hpp"
#include <cmath>
#include <cstring>

namespace Ge
{
	class Textures
	{
	public:
		Textures(stbi_uc* pc, int Width,int Height,int ind,bool filter, VulkanMisc * vM);
		~Textures();
		VkImageView getVkImageView();
		VkSampler getVkSampler();
		int getIndex();
	public:
		static void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels, uint32_t layerCount, VulkanMisc * vM);
		static void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels, uint32_t layerCount, VulkanMisc * vM);
	private:
		int texWidth, texHeight;
		stbi_uc* pixelContainer;
		uint32_t mipLevels;
		VmaBufferImage textureImage;
		VkDeviceSize imageSize;	
		VkImageView textureImageView;
		VkSampler textureSampler;
		VulkanDeviceMisc * vulkanM;
		int index = 0;
		float minLod = 0.0f;
	};
}

#endif // __ENGINE_TEXTURES__
