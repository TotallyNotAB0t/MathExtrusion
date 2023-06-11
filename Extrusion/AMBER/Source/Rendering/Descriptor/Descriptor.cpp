#include "Descriptor.hpp"

namespace Ge
{
	int Descriptor::countDescriptor = 0;
	std::vector<Descriptor *> Descriptor::s_Descriptor;
    Descriptor::Descriptor(VulkanMisc *vM, VkDescriptorType descriptorType, int baseCount, bool ignoreDescriptorList)
    {
        m_count = baseCount;                
        vulkanM = vM;
		m_countDescriptor = countDescriptor;
		countDescriptor++;
        m_descriptorType = descriptorType;
        m_DescriptorSetLayout = createVkDescriptorSetLayout(vM,baseCount,descriptorType, m_countDescriptor);
        m_DescriptorPool = createVkDescriptorPool(vM,baseCount,descriptorType); 
        m_DescriptorSets = createVkDescriptorSet(vM,m_DescriptorSetLayout,m_DescriptorPool); 
        if (!ignoreDescriptorList)
        {
            s_Descriptor.push_back(this);
        }
    }

	std::vector<Descriptor *> Descriptor::GetAllDescriptor()
	{
		return Descriptor::s_Descriptor;
	}

	Descriptor::~Descriptor()
	{
		s_Descriptor.erase(std::remove(s_Descriptor.begin(), s_Descriptor.end(), this), s_Descriptor.end());
		destroyVkDescriptorSet(vulkanM,m_DescriptorSets,m_DescriptorPool);
		destroyVkDescriptorPool(vulkanM, m_DescriptorPool);
		destroyVkVkDescriptorSetLayout(vulkanM, m_DescriptorSetLayout);
	}

    VkDescriptorSetLayout Descriptor::getDescriptorSetLayout()
    {
        return m_DescriptorSetLayout;
    }

    VkDescriptorPool Descriptor::getDescriptorPool()
    {
        return m_DescriptorPool;
    }

    VkDescriptorSet Descriptor::getDescriptorSets()
    {
        return m_DescriptorSets;
    }

    void Descriptor::updateCount(VulkanMisc *vM, int count, std::vector<VkDescriptorBufferInfo> bufferInfo)
    {
        if(m_count != count)
        {
            vkQueueWaitIdle(vulkanM->str_VulkanDeviceMisc->str_graphicsQueue);
            destroyVkDescriptorPool(vulkanM,m_DescriptorPool);
            destroyVkVkDescriptorSetLayout(vulkanM,m_DescriptorSetLayout);
            m_DescriptorSetLayout = createVkDescriptorSetLayout(vM,count,m_descriptorType, m_countDescriptor);
            m_DescriptorPool = createVkDescriptorPool(vM,count,m_descriptorType);
            m_DescriptorSets = createVkDescriptorSet(vM,m_DescriptorSetLayout,m_DescriptorPool);   		
        }
        m_count = count;
        VkWriteDescriptorSet descriptorWrites{};
        descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites.dstSet = m_DescriptorSets;
        descriptorWrites.dstBinding = 0;
        descriptorWrites.dstArrayElement = 0;
        descriptorWrites.descriptorType = m_descriptorType;
        descriptorWrites.descriptorCount = static_cast<uint32_t>(bufferInfo.size());
        descriptorWrites.pBufferInfo = bufferInfo.data();
        vkUpdateDescriptorSets(vM->str_VulkanDeviceMisc->str_device, static_cast<uint32_t>(1), &descriptorWrites, 0, nullptr);
		vM->str_VulkanDescriptor->recreateCommandBuffer = true;
    }

    void Descriptor::updateCount(VulkanMisc *vM, int count, std::vector<VkDescriptorImageInfo> bufferInfo)
    {
        if(m_count != count)
        {
            vkQueueWaitIdle(vulkanM->str_VulkanDeviceMisc->str_graphicsQueue);
            destroyVkDescriptorPool(vulkanM,m_DescriptorPool);
            destroyVkVkDescriptorSetLayout(vulkanM,m_DescriptorSetLayout);
            m_DescriptorSetLayout = createVkDescriptorSetLayout(vM,count,m_descriptorType, m_countDescriptor);
            m_DescriptorPool = createVkDescriptorPool(vM,count,m_descriptorType);
            m_DescriptorSets = createVkDescriptorSet(vM,m_DescriptorSetLayout,m_DescriptorPool);
			vM->str_VulkanDescriptor->recreateCommandBuffer = true;
        }
        m_count = count;
        VkWriteDescriptorSet descriptorWrites{};
        descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites.dstSet = m_DescriptorSets;
        descriptorWrites.dstBinding = 0;
        descriptorWrites.dstArrayElement = 0;
        descriptorWrites.descriptorType = m_descriptorType;
        descriptorWrites.descriptorCount = static_cast<uint32_t>(bufferInfo.size());
        descriptorWrites.pImageInfo = bufferInfo.data();
        vkUpdateDescriptorSets(vM->str_VulkanDeviceMisc->str_device, static_cast<uint32_t>(1), &descriptorWrites, 0, nullptr);
    }

    VkDescriptorPool Descriptor::createVkDescriptorPool(VulkanMisc *vM,int count,VkDescriptorType type)
    {
        VkDescriptorPool m_descriptorPool;
        VkDescriptorPoolSize poolSizes{};
        poolSizes.type = type;
        poolSizes.descriptorCount = static_cast<uint32_t>(vM->str_VulkanSwapChainMisc->str_swapChainImages.size()*count);
        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSizes;
        poolInfo.maxSets = static_cast<uint32_t>(vM->str_VulkanSwapChainMisc->str_swapChainImages.size());

        if (vkCreateDescriptorPool(vM->str_VulkanDeviceMisc->str_device, &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS)
        {
            Debug::Error("Echec de la creation d'un descriptor pool");
        }              
        return m_descriptorPool;
    }

    VkDescriptorSetLayout Descriptor::createVkDescriptorSetLayout(VulkanMisc *vM,int count,VkDescriptorType type,int descriptorCount)
    {
        VkDescriptorSetLayout m_descriptorSetLayout;
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = static_cast<uint32_t>(count);
        uboLayoutBinding.descriptorType = type;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;

        if (vkCreateDescriptorSetLayout(vM->str_VulkanDeviceMisc->str_device, &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
        {
            Debug::Error("Echec de la creation du descriptor set layout");
        }    
		if (vM->str_VulkanSwapChainMisc->str_descriptorSetLayout.size() != Descriptor::countDescriptor)
		{
			vM->str_VulkanSwapChainMisc->str_descriptorSetLayout.push_back(m_descriptorSetLayout);
		}
		else
		{
			vM->str_VulkanSwapChainMisc->str_descriptorSetLayout[descriptorCount] = m_descriptorSetLayout;
		}
        return m_descriptorSetLayout;
    }

    VkDescriptorSet Descriptor::createVkDescriptorSet(VulkanMisc *vM,VkDescriptorSetLayout descriptorSetLayout,VkDescriptorPool descriptorPool)
    {
        VkDescriptorSet m_descriptorSets; 
        VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(1);
		allocInfo.pSetLayouts = &descriptorSetLayout;

		if (vkAllocateDescriptorSets(vM->str_VulkanDeviceMisc->str_device, &allocInfo, &m_descriptorSets) != VK_SUCCESS)
		{
			Debug::Error("Echec de l'allocation du descriptor sets");			
		}
        return m_descriptorSets;
    }

    void Descriptor::destroyVkDescriptorSet(VulkanMisc *vM, VkDescriptorSet descriptorSets,VkDescriptorPool descriptorPool)
    {
        //vkFreeDescriptorSets(vM->str_VulkanDeviceMisc->str_device, descriptorPool, 1, &descriptorSets);
    }

    void Descriptor::destroyVkDescriptorPool(VulkanMisc *vM,VkDescriptorPool descriptorPool)
    {
        vkDestroyDescriptorPool(vM->str_VulkanDeviceMisc->str_device,descriptorPool,nullptr);
    }

    void Descriptor::destroyVkVkDescriptorSetLayout(VulkanMisc *vM,VkDescriptorSetLayout descriptorSetLayout)
    {        
        vkDestroyDescriptorSetLayout(vM->str_VulkanDeviceMisc->str_device, descriptorSetLayout, nullptr);
    }
}