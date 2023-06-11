#ifndef __ENGINE_IMAGE_VIEW__
#define __ENGINE_IMAGE_VIEW__

#include "Debug.hpp"
#include "VulkanMisc.hpp"

namespace Ge
{
    class ImageViewSwapChains
    {
    public:
        ImageViewSwapChains(VkImage * SwapChainImage, VkFormat swapChainImageFormat, VulkanMisc * vM);
        ~ImageViewSwapChains();
        VkImageView getImageView();
		static VkImageView createImageView(VkImage image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels, VulkanMisc *vM);
    private:
        VkImageView m_imageView;
        VulkanMisc * vulkanM;
    };
}

#endif //__ENGINE_IMAGE_VIEW__