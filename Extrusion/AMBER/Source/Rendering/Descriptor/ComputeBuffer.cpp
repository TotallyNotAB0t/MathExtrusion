#include "ComputeBuffer.hpp"

namespace Ge
{
	ComputeBuffer::ComputeBuffer(VulkanMisc* vM, int baseCount, int sizeofR)
	{
		vulkanM = vM;
		m_count = baseCount;
		m_sizeOfResource = sizeofR;
		m_DescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorCount = 1; // Utilisation d'un seul buffer
		uboLayoutBinding.descriptorType = m_DescriptorType;
		uboLayoutBinding.pImmutableSamplers = nullptr;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &uboLayoutBinding;

		if (vkCreateDescriptorSetLayout(vM->str_VulkanDeviceMisc->str_device, &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation du descriptor set layout");
		}

		VkDescriptorPoolSize poolSizes{};
		poolSizes.type = m_DescriptorType;
		poolSizes.descriptorCount = 1; // Utilisation d'un seul buffer
		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSizes;
		poolInfo.maxSets = static_cast<uint32_t>(vM->str_VulkanSwapChainMisc->str_swapChainImages.size());

		if (vkCreateDescriptorPool(vM->str_VulkanDeviceMisc->str_device, &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation d'un descriptor pool");
		}

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &m_DescriptorSetLayout;

		if (vkAllocateDescriptorSets(vM->str_VulkanDeviceMisc->str_device, &allocInfo, &m_DescriptorSets) != VK_SUCCESS)
		{
			Debug::Error("Echec de l'allocation du descriptor sets");
		}

		if (!BufferManager::createBuffer(m_count *m_sizeOfResource, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_vmaBuffer, vM->str_VulkanDeviceMisc))
		{
			Debug::Error("Echec de la creation d'un buffer storage");
		}

		VkDescriptorBufferInfo bufferI{};
		bufferI.buffer = m_vmaBuffer.buffer;
		bufferI.offset = 0;
		bufferI.range = m_count*m_sizeOfResource;

		VkWriteDescriptorSet descriptorWrites{};
        descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites.dstSet = m_DescriptorSets;
        descriptorWrites.dstBinding = 0;
        descriptorWrites.dstArrayElement = 0;
        descriptorWrites.descriptorType = m_DescriptorType;
        descriptorWrites.descriptorCount = 1;
        descriptorWrites.pBufferInfo = &bufferI;
        vkUpdateDescriptorSets(vM->str_VulkanDeviceMisc->str_device, static_cast<uint32_t>(1), &descriptorWrites, 0, nullptr);
	}

	void ComputeBuffer::SetData(void* data, int managedBufferStartIndex, int computeBufferStartIndex, int count)
	{
		VkDeviceSize bufferSize = m_sizeOfResource * count;
		VkDeviceSize offset = m_sizeOfResource * computeBufferStartIndex;

		if (offset + bufferSize > m_sizeOfResource * m_count)
		{
			Debug::Error("Dépassement de mémoire lors de l'appel à SetData");
			return;
		}

		void* mappedData = BufferManager::mapMemory(m_vmaBuffer);

		memcpy(static_cast<char*>(mappedData) + offset, data, bufferSize);

		BufferManager::unMapMemory(m_vmaBuffer);
	}

	VkDescriptorSetLayout ComputeBuffer::getDescriptorSetLayout()
	{
		return m_DescriptorSetLayout;
	}

	VkDescriptorSet ComputeBuffer::getDescriptorSet()
	{
		return m_DescriptorSets;
	}

	void ComputeBuffer::GetData(void* data, int managedBufferStartIndex, int computeBufferStartIndex, int count)
	{
		VkDeviceSize bufferSize = m_sizeOfResource * count;
		VkDeviceSize offset = m_sizeOfResource * computeBufferStartIndex;

		if (offset + bufferSize > m_sizeOfResource * m_count)
		{
			Debug::Error("Dépassement de mémoire lors de l'appel à GetData");
			return;
		}

		void* mappedData = BufferManager::mapMemory(m_vmaBuffer);

		memcpy(data, static_cast<char*>(mappedData) + offset, bufferSize);

		BufferManager::unMapMemory(m_vmaBuffer);
	}

	ComputeBuffer::~ComputeBuffer()
	{
		BufferManager::destroyBuffer(m_vmaBuffer);
		vkDestroyDescriptorPool(vulkanM->str_VulkanDeviceMisc->str_device, m_DescriptorPool, nullptr);
		vkDestroyDescriptorSetLayout(vulkanM->str_VulkanDeviceMisc->str_device, m_DescriptorSetLayout, nullptr);
	}
}