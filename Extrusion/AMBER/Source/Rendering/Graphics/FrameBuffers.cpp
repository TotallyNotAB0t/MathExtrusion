#include "FrameBuffers.hpp"

namespace Ge
{
	bool FrameBuffers::initialize(VulkanMisc * vM)
	{
		vulkanM = vM;
		std::vector<VkImageView> swapChainImageViews = vM->str_VulkanSwapChainMisc->str_swapChainImageViews;
		VkRenderPass renderPass = vM->str_VulkanSwapChainMisc->str_renderPass;
		VkExtent2D swapChainExtent = vM->str_VulkanSwapChainMisc->str_swapChainExtent;
		VkImageView colorImageView = vM->str_VulkanCommandeBufferMisc->str_colorImageView;
		VkImageView depthImageView = vM->str_VulkanCommandeBufferMisc->str_depthImageView;
		m_swapChainFramebuffers.resize(swapChainImageViews.size());
		
		for (size_t i = 0; i < swapChainImageViews.size(); i++)
		{
			std::array<VkImageView, 3> attachments = {
				colorImageView,
				depthImageView,
				swapChainImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = swapChainExtent.width;
			framebufferInfo.height = swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(vM->str_VulkanDeviceMisc->str_device, &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS)
			{
				Debug::Error("Echec de la creation d'un framebuffer");
				return false;
			}
		}
		vM->str_VulkanCommandeBufferMisc->str_swapChainFramebuffers = m_swapChainFramebuffers;
		Debug::INITSUCCESS("FrameBuffers");
		return true;
	}

	void FrameBuffers::release()
	{
		for (auto framebuffer : m_swapChainFramebuffers)
		{
			vkDestroyFramebuffer(vulkanM->str_VulkanDeviceMisc->str_device, framebuffer, nullptr);
		}
		Debug::RELEASESUCCESS("FrameBuffers");
	}
}