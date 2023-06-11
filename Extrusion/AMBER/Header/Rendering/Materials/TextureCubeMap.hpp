#ifndef __ENGINE_TEXTURE_CUBE_MAP__
#define __ENGINE_TEXTURE_CUBE_MAP__

#include "Textures.hpp"

namespace Ge
{
	class TextureCubeMap
	{
	public:
		TextureCubeMap(std::vector<unsigned char *> pc, int Width, int Height, int ind,bool filter, VulkanMisc * vM);
		~TextureCubeMap();
		VkImageView getVkImageView();
		VkSampler getVkSampler();
		int getIndex();
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


#endif //__ENGINE_TEXTURE_CUBE_MAP__