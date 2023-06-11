#include "SyncObjects.hpp"

namespace Ge
{
	bool SyncObjects::initialize(VulkanMisc * vM)
	{
		VkDevice device = vM->str_VulkanDeviceMisc->str_device;
		vulkanM = vM;
		m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		m_imagesInFlight.resize(vM->str_VulkanSwapChainMisc->str_swapChainImages.size(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(device, &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS) 
			{
				Debug::Error("Echec de creation des objets de synchronisation pour une image");
			}
		}
		vM->str_VulkanSynchronisation->MAX_FRAMES_IN_FLIGHT = MAX_FRAMES_IN_FLIGHT;
		vM->str_VulkanSynchronisation->str_imageAvailableSemaphores = m_imageAvailableSemaphores;
		vM->str_VulkanSynchronisation->str_imagesInFlight = m_imagesInFlight;
		vM->str_VulkanSynchronisation->str_inFlightFences = m_inFlightFences;
		vM->str_VulkanSynchronisation->str_renderFinishedSemaphores = m_renderFinishedSemaphores;
		Debug::INITSUCCESS("SyncObjects");
		return true;
	}

	void SyncObjects::release()
	{
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(vulkanM->str_VulkanDeviceMisc->str_device, m_renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(vulkanM->str_VulkanDeviceMisc->str_device, m_imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(vulkanM->str_VulkanDeviceMisc->str_device, m_inFlightFences[i], nullptr);
		}
		Debug::RELEASESUCCESS("SyncObjects");
	}
}
