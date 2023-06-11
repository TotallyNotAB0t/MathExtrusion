#define VMA_IMPLEMENTATION
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_core.h"
#include "BufferManager.hpp"

namespace Ge
{
	BufferManager * BufferManager::s_pInstance = nullptr;

	static constexpr uint32_t GetVulkanApiVersion()
	{
#if VMA_VULKAN_VERSION == 1002000
		return VK_API_VERSION_1_2;
#elif VMA_VULKAN_VERSION == 1001000
		return VK_API_VERSION_1_1;
#elif VMA_VULKAN_VERSION == 1000000
		return VK_API_VERSION_1_0;
#else
#error Invalid VMA_VULKAN_VERSION.
		return UINT32_MAX;
#endif
	}

	bool BufferManager::initialize(VulkanMisc * vM)
	{
		VmaAllocatorCreateInfo allocatorInfo = {};		
		allocatorInfo.vulkanApiVersion = GetVulkanApiVersion();
		allocatorInfo.physicalDevice = vM->str_VulkanDeviceMisc->str_physicalDevice;
		allocatorInfo.device = vM->str_VulkanDeviceMisc->str_device;
		allocatorInfo.instance = vM->str_VulkanDeviceMisc->str_instance;
		allocatorInfo.flags = VMA_ALLOCATOR_CREATE_KHR_DEDICATED_ALLOCATION_BIT | VMA_ALLOCATOR_CREATE_KHR_BIND_MEMORY2_BIT | VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT | VMA_ALLOCATOR_CREATE_AMD_DEVICE_COHERENT_MEMORY_BIT;//VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT

		if (vmaCreateAllocator(&allocatorInfo, &m_allocator) != VK_SUCCESS)
		{
			Debug::Error("Echec de creation d'un vulkan memory allocator");
			return false;
		}
		BufferManager::s_pInstance = this;
		Debug::Info("Initialisation du BufferManager");
		return true;
	}

	void BufferManager::release()
	{
		vmaDestroyAllocator(m_allocator);
		s_pInstance = nullptr;
		Debug::Info("Liberation du BufferManager");
	}

	bool BufferManager::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags flags, VmaBuffer & buffer, VulkanDeviceMisc * vM)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(vM->str_device, &bufferInfo, nullptr, &buffer.buffer) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation d'un vma buffer");
			return false;
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(vM->str_device, buffer.buffer, &memRequirements);

		VmaAllocationCreateInfo allocation_info = VmaAllocationCreateInfo{};
		allocation_info.requiredFlags = flags;

		buffer.allocation = VmaAllocation{};
		if (vmaAllocateMemory(s_pInstance->m_allocator, &memRequirements, &allocation_info, &buffer.allocation, nullptr) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation d'une vma allocation");
			return false;
		}

		if (vmaBindBufferMemory(s_pInstance->m_allocator, buffer.allocation, buffer.buffer))
		{
			Debug::Error("Echec du bind du BufferMemory");
			return false;
		}

		return true;
	}

	bool BufferManager::createImageBuffer(uint32_t width, uint32_t height, VkImageType imageType, uint32_t arrayLayers, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VmaBufferImage& image, VkImageCreateFlags flags, VulkanMisc * vM, VkImageLayout initiallayout)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = imageType;//VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = mipLevels;
		imageInfo.arrayLayers = arrayLayers;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = initiallayout;
		imageInfo.usage = usage;
		imageInfo.samples = numSamples;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = flags;

		if (vkCreateImage(vM->str_VulkanDeviceMisc->str_device, &imageInfo, nullptr, &image.image) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation d'une image");
			return false;
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(vM->str_VulkanDeviceMisc->str_device, image.image, &memRequirements);

		VmaAllocationCreateInfo allocation_info = VmaAllocationCreateInfo{};
		//allocation_info.requiredFlags = flags;

		image.allocation = VmaAllocation{};
		if (vmaAllocateMemory(s_pInstance->m_allocator, &memRequirements, &allocation_info, &image.allocation, nullptr) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation d'une vma allocation");
			return false;
		}
	
		if (vmaBindImageMemory(s_pInstance->m_allocator, image.allocation, image.image))
		{
			Debug::Error("Echec du bind du BufferImageMemory");
			return false;
		}
		return true;
	}

	void BufferManager::destroyBuffer(VmaBuffer & buffer)
	{
		vmaDestroyBuffer(s_pInstance->m_allocator, buffer.buffer, buffer.allocation);
	}

	void BufferManager::destroyImageBuffer(VmaBufferImage & buffer)
	{
		vmaDestroyImage(s_pInstance->m_allocator, buffer.image, buffer.allocation);
	}

	void * BufferManager::mapMemory(VmaBuffer & buffer)
	{
		void * data;
		vmaMapMemory(s_pInstance->m_allocator, buffer.allocation, &data);		
		return data;
	}

	void BufferManager::unMapMemory(VmaBuffer & buffer)
	{
		vmaUnmapMemory(s_pInstance->m_allocator, buffer.allocation);
	}

	void BufferManager::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VulkanMisc * vM)
	{
		VkCommandBuffer commandBuffer = BufferManager::beginSingleTimeCommands(vM);

		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		BufferManager::endSingleTimeCommands(commandBuffer, vM);
	}

	void BufferManager::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t baseArrayLayer, uint32_t bufferOffset,  VulkanMisc * vM)
	{
		VkCommandBuffer commandBuffer = BufferManager::beginSingleTimeCommands(vM);

		VkBufferImageCopy region{};
		region.bufferOffset = bufferOffset;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = baseArrayLayer;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};

		vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		BufferManager::endSingleTimeCommands(commandBuffer, vM);
	}

	VkCommandBuffer BufferManager::beginSingleTimeCommands(VulkanMisc * vM)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = vM->str_VulkanCommandeBufferMisc->str_commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(vM->str_VulkanDeviceMisc->str_device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void BufferManager::endSingleTimeCommands(VkCommandBuffer commandBuffer, VulkanMisc * vM)
	{
		VkQueue graphicsQueue = vM->str_VulkanDeviceMisc->str_graphicsQueue;
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);

		vkFreeCommandBuffers(vM->str_VulkanDeviceMisc->str_device, vM->str_VulkanCommandeBufferMisc->str_commandPool, 1, &commandBuffer);
	}
}