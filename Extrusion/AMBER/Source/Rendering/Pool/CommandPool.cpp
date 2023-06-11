#include "CommandPool.hpp"
#include "PhysicalDevices.hpp"

namespace Ge
{
	bool CommandPool::initialize(VulkanMisc * vM)
	{
		vulkanM = vM;
		QueueFamilyIndices queueFamilyIndices = PhysicalDevices::findQueueFamilies(vM->str_VulkanDeviceMisc->str_physicalDevice,vM);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		if (vkCreateCommandPool(vM->str_VulkanDeviceMisc->str_device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation de la command pool");
			return false;
		}
		vM->str_VulkanCommandeBufferMisc->str_commandPool = m_commandPool;
		Debug::INITSUCCESS("CommandPool");
		return true;
	}

	void CommandPool::release()
	{		
		vkDestroyCommandPool(vulkanM->str_VulkanDeviceMisc->str_device, m_commandPool, nullptr);
		Debug::RELEASESUCCESS("CommandPool");
	}
}