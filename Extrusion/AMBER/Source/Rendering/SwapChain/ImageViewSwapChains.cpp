#include "ImageViewSwapChains.hpp"

namespace Ge
{

    ImageViewSwapChains::ImageViewSwapChains(VkImage * SwapChainImage, VkFormat swapChainImageFormat, VulkanMisc * vM)
    {
        vulkanM = vM;
        m_imageView = createImageView((*SwapChainImage), VK_IMAGE_VIEW_TYPE_2D, swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, vM);
    }

    ImageViewSwapChains::~ImageViewSwapChains()
    {
        vkDestroyImageView(vulkanM->str_VulkanDeviceMisc->str_device, m_imageView, nullptr);
    }

    VkImageView ImageViewSwapChains::getImageView()
    {
        return m_imageView;
    }

    VkImageView ImageViewSwapChains::createImageView(VkImage image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels, VulkanMisc *vM)
    {
        VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = viewType;//VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		if (vkCreateImageView(vM->str_VulkanDeviceMisc->str_device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation d'une image vue");
		}

		return imageView;
    }
}