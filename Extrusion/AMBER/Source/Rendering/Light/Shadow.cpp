#include "Shadow.hpp"
#include "ImageViewSwapChains.hpp"

namespace Ge
{
	Shadow::Shadow(VkImageView depth, VkRenderPass renderPass,Transform* lightTransform, VulkanMisc* vM)
	{
		vMisc = vM;
		m_lightTransform = lightTransform;
		
		VkCommandBuffer commandBuffer = BufferManager::beginSingleTimeCommands(vMisc);
		VmaBuffer buffer;
		if (!BufferManager::createBuffer(sizeof(ShadowMatrix), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, buffer, vM->str_VulkanDeviceMisc))
		{
			Debug::Error("Echec de la creation d'un uniform buffer");
		}
		m_vmaUniformBuffers.push_back(buffer);

		BufferManager::createImageBuffer(TEXTURE_DIM, TEXTURE_DIM, VK_IMAGE_TYPE_2D, 1, 1, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R32_SFLOAT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depthTexture, 0, vM);

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.layerCount = 1;
		VkImageMemoryBarrier imageMemoryBarrier{};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		imageMemoryBarrier.image = m_depthTexture.image;
		imageMemoryBarrier.subresourceRange = subresourceRange;

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
			VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &imageMemoryBarrier);
		BufferManager::endSingleTimeCommands(commandBuffer,vMisc);

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = m_depthTexture.image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = VK_FORMAT_R32_SFLOAT;
		viewInfo.components = { VK_COMPONENT_SWIZZLE_R };
		viewInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		if (vkCreateImageView(vM->str_VulkanDeviceMisc->str_device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation d'une image vue");
		}

		m_depthTextureView.push_back(imageView);
		for (int i = 0; i < m_depthTextureView.size();i++)
		{
			ShadowMatrix pc_empty;
			pc_empty.pos = glm::vec3(0.0f, 0.0f, 0.0f);
			pc_empty.proj = glm::mat4();
			pc_empty.view = glm::mat4();
			m_pushConstantShadow.push_back(pc_empty);
		}
		createFrameBuffer(depth,renderPass);

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_NEAREST;
		samplerInfo.minFilter = VK_FILTER_NEAREST;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16.0f;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = VK_SAMPLE_COUNT_1_BIT;
		samplerInfo.mipLodBias = 0.0f;
		if (vkCreateSampler(vM->str_VulkanDeviceMisc->str_device, &samplerInfo, nullptr, &m_textureSampler) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation d'un sampler de texture");
		}
		mapMemory();
	}

	std::vector<VkImageView> Shadow::getImageView()
	{
		return m_depthTextureView;
	}

	std::vector<VkBuffer> Shadow::getUniformBuffers()
	{
		std::vector<VkBuffer> buffers;
		for (int i = 0; i < m_vmaUniformBuffers.size(); i++)
		{
			buffers.push_back(m_vmaUniformBuffers[i].buffer);
		}
		return buffers;
	}

	VkSampler Shadow::getImageSampler()
	{
		return m_textureSampler;
	}

	std::vector<std::vector<VkFramebuffer>> Shadow::getFrameBuffer()
	{
		return m_frameBuffer;
	}

	void Shadow::createFrameBuffer(VkImageView depth, VkRenderPass renderPass)
	{
		for (int i = 0; i < m_frameBuffer.size(); i++)
		{
			for (int j = 0; j < m_frameBuffer[i].size(); j++)
			{
				vkDestroyFramebuffer(vMisc->str_VulkanDeviceMisc->str_device, m_frameBuffer[i][j], nullptr);
			}
		}
		m_frameBuffer.clear();
		std::vector<VkImageView> swapChainImageViews = vMisc->str_VulkanSwapChainMisc->str_swapChainImageViews;
		VkImageView attachments[2];
		attachments[1] = depth;
		VkFramebuffer frameBuffer;
		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 2;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = TEXTURE_DIM;
		framebufferInfo.height = TEXTURE_DIM;
		framebufferInfo.layers = 1;
		for (int j = 0; j < m_depthTextureView.size(); j++)
		{
			attachments[0] = m_depthTextureView[j];
			std::vector<VkFramebuffer> frame;
			for (size_t i = 0; i < swapChainImageViews.size(); i++)
			{
				if (vkCreateFramebuffer(vMisc->str_VulkanDeviceMisc->str_device, &framebufferInfo, nullptr, &frameBuffer) != VK_SUCCESS)
				{
					Debug::Error("Echec de la creation d'un framebuffer");
				}
				frame.push_back(frameBuffer);
			}
			m_frameBuffer.push_back(frame);
		}
	}

	float Shadow::aspectRatio() const
	{
		return 1.0f;
	}

	void Shadow::mapMemory()
	{
		for (int i = 0; i < m_pushConstantShadow.size(); i++)
		{
			UpdateUniformBuffer(i);
			memcpy(BufferManager::mapMemory(m_vmaUniformBuffers[i]), &m_pushConstantShadow[i], sizeof(ShadowMatrix));
			BufferManager::unMapMemory(m_vmaUniformBuffers[i]);
		}
	}

	void Shadow::UpdateUniformBuffer(int frame)
	{
		m_pushConstantShadow[frame].pos = m_lightTransform->position;
		m_pushConstantShadow[frame].view = glm::inverse(glm::translate(glm::mat4(1.0f), m_lightTransform->position) * glm::toMat4(glm::quat(m_lightTransform->rotation)) * glm::scale(glm::mat4(1.0f), m_lightTransform->scale));
		glm::mat4 projectionMatrix;
		bool m_ortho = false;
		float m_orthoSize = 10.0f;
		float m_near = 0.01f;
		float m_far = 1000.0f;
		float m_fov = 60.0f;
		if (m_ortho)
		{
			float halfHeight = m_orthoSize * 0.5f;
			float halfWidth = halfHeight * aspectRatio();
			projectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, m_near, m_far);
		}
		else
		{
			projectionMatrix = glm::perspective(glm::radians(m_fov), aspectRatio(), m_near, m_far);
		}

		m_pushConstantShadow[frame].proj = glm::scale(projectionMatrix, glm::vec3(1.0f, -1.0f, 1.0f));
	}

	Shadow::~Shadow()
	{		
		for (int i = 0; i < m_frameBuffer.size(); i++)
		{
			for (int j = 0; j < m_frameBuffer[i].size(); j++)
			{
				vkDestroyFramebuffer(vMisc->str_VulkanDeviceMisc->str_device, m_frameBuffer[i][j], nullptr);
			}
		}
		m_frameBuffer.clear();
		for (int i = 0; i < m_depthTextureView.size(); i++)
		{
			vkDestroyImageView(vMisc->str_VulkanDeviceMisc->str_device, m_depthTextureView[i], nullptr);
		}		
		vkDestroySampler(vMisc->str_VulkanDeviceMisc->str_device, m_textureSampler, nullptr);
		m_depthTextureView.clear();
		BufferManager::destroyImageBuffer(m_depthTexture);
		for (int i = 0; i < m_vmaUniformBuffers.size(); i++)
		{
			BufferManager::destroyBuffer(m_vmaUniformBuffers[i]);
		}
		m_vmaUniformBuffers.clear();
	}
}