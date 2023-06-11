#include "RenderingEngine.hpp"

namespace Ge
{
    RenderingEngine::RenderingEngine()
    {
        m_vulkanMisc.str_VulkanDeviceMisc = &m_vulkanDeviceMisc;
        m_vulkanMisc.str_VulkanSwapChainMisc = &m_vulkanSwapChainMisc;
        m_vulkanMisc.str_VulkanDescriptor = &m_VulkanDescriptor;
        m_vulkanMisc.str_VulkanCommandeBufferMisc = &m_VulkanCommandeBufferMisc;
		m_vulkanMisc.str_VulkanSynchronisation = &m_VulkanSynchronisation;
    }

    bool RenderingEngine::initialize(ptrClass *p_ptrClass)
    {
        if (p_ptrClass == nullptr)
        {
            Debug::Error("ptrClass nullptr RenderingEngine");
            return false;
        }
		m_ptrClass = p_ptrClass;
		p_ptrClass->cameraManager = &m_cameraManager;		
		p_ptrClass->lightManager = &m_lightManager;
		p_ptrClass->materialManager = &m_materialManager;
		p_ptrClass->modelManager = &m_modelManager;
		p_ptrClass->textureManager = &m_textureManager;
		p_ptrClass->hud = &m_hud;
		p_ptrClass->skyboxManager = &m_skyboxManager;
        p_ptrClass->shadowManager = &m_shadowManager;
        Debug::Info("Initialisation du moteur de rendu");
        if (!RenderingEngine::m_window.initialize(p_ptrClass->settingManager->getWindowWidth(), p_ptrClass->settingManager->getWindowHeight(), p_ptrClass->settingManager->getName(), p_ptrClass->settingManager->getIconPath(), &m_vulkanMisc))
        {
            Debug::INITFAILED("Window");
            return false;
        }        
        if (!RenderingEngine::m_instanceVulkan.initialize(p_ptrClass->settingManager->getName(), p_ptrClass->settingManager->getVersion(), &m_vulkanMisc))
        {
            Debug::INITFAILED("InstanceVulkan");
            return false;
        }
        if (!RenderingEngine::m_validationLayer.initialize(&m_vulkanMisc))
        {
            Debug::INITFAILED("ValidationLayer");
            return false;
        }
        if (!RenderingEngine::m_windowSurface.initialize(&m_vulkanMisc))
        {
            Debug::INITFAILED("WindowSurface");
            return false;
        }
        if (!RenderingEngine::m_physicalDevice.initialize(&m_vulkanMisc))
        {
            Debug::INITFAILED("PhysicalDevice");
            return false;
        }
        if (!RenderingEngine::m_logicalDevice.initialize(&m_vulkanMisc, &m_windowSurface))
        {
            Debug::INITFAILED("LogicalDevice");
            return false;
        }
        if (!RenderingEngine::m_commandPool.initialize(&m_vulkanMisc))
        {
            Debug::INITFAILED("CommandPool");
            return false;
        }
        if (!RenderingEngine::m_bufferManager.initialize(&m_vulkanMisc))
        {
            Debug::INITFAILED("BufferManager");
            return false;
        }
        if (!RenderingEngine::m_swapChain.initialize(&m_vulkanMisc, p_ptrClass,&m_shaderUniformBufferDivers))
        {
            Debug::INITFAILED("SwapChain");
            return false;
        }
		m_vulkanMisc.str_VulkanSwapChainMisc->swapChainRecreate = &m_swapChain;
        if (!RenderingEngine::m_colorResources.initialize(&m_vulkanMisc))
        {
            Debug::INITFAILED("ColorResources");
            return false;
        }
        if (!RenderingEngine::m_depthResources.initialize(&m_vulkanMisc))
        {
            Debug::INITFAILED("DepthResources");
            return false;
        }
        if (!RenderingEngine::m_frameBuffers.initialize(&m_vulkanMisc))
        {
            Debug::INITFAILED("Framebuffers");
            return false;
        }
        if(!RenderingEngine::m_cameraManager.initialize(&m_vulkanMisc, p_ptrClass->inputManager))
        {
            Debug::INITFAILED("CameraManager");
            return false;
        }
        if (!RenderingEngine::m_textureManager.initialize(&m_vulkanMisc))
        {
            Debug::INITFAILED("TextureManager");
            return false;
        }
        if (!RenderingEngine::m_modelManager.initiliaze(&m_vulkanMisc))
        {
            Debug::INITFAILED("ModelManager");
            return false;
        }
        if (!RenderingEngine::m_materialManager.initialize(&m_vulkanMisc))
        {
            Debug::INITFAILED("MaterialManager");
            return false;
        }
        if (!RenderingEngine::m_lightManager.initialize(&m_vulkanMisc))
        {
            Debug::INITFAILED("LightManager");
            return false;
        }
        if (!RenderingEngine::m_shaderUniformBufferDivers.initialize(&m_vulkanMisc, p_ptrClass->settingManager))
        {
            Debug::INITFAILED("ShaderUniformBufferDivers");
            return false;
        }
        if (!RenderingEngine::m_shadowManager.initialize(&m_vulkanMisc))
        {
            Debug::INITFAILED("ShadowManager");
            return false;
        }
		if (!RenderingEngine::m_skyboxManager.initialize(&m_vulkanMisc, &m_modelManager, p_ptrClass->graphiquePipelineManager))
		{
			Debug::INITFAILED("SkyboxManager");
			return false;
		}
        if (!RenderingEngine::m_commandBuffer.initialize(&m_shadowManager,&m_vulkanMisc, p_ptrClass))
		{
            Debug::INITFAILED("CommandBuffer");
            return false;
        }
		if (!RenderingEngine::m_syncObjects.initialize(&m_vulkanMisc))
		{
			Debug::INITFAILED("SyncObject");
			return false;
		}
		if (!RenderingEngine::m_hud.initialize(&m_vulkanMisc))
		{
			Debug::INITFAILED("hud");
			return false;
		}
        Debug::INITSUCCESS("RenderingEngine");

        return true;
    }

    void RenderingEngine::release()
    {
        vkDeviceWaitIdle(m_vulkanDeviceMisc.str_device);    
		RenderingEngine::m_hud.release();
		RenderingEngine::m_syncObjects.release();
		RenderingEngine::m_commandBuffer.release();
		RenderingEngine::m_skyboxManager.release();
        RenderingEngine::m_shadowManager.release();
        RenderingEngine::m_shaderUniformBufferDivers.release();
        RenderingEngine::m_lightManager.release();
		RenderingEngine::m_modelManager.release();
        RenderingEngine::m_materialManager.release();        
		RenderingEngine::m_cameraManager.release();
        RenderingEngine::m_textureManager.release();
        RenderingEngine::m_frameBuffers.release();
        RenderingEngine::m_depthResources.release();
        RenderingEngine::m_colorResources.release();
        RenderingEngine::m_swapChain.release();
        RenderingEngine::m_bufferManager.release();
        RenderingEngine::m_commandPool.release();
        RenderingEngine::m_logicalDevice.release();
        RenderingEngine::m_physicalDevice.release();
        RenderingEngine::m_windowSurface.release();
        RenderingEngine::m_validationLayer.release();
        RenderingEngine::m_instanceVulkan.release();
        RenderingEngine::m_window.release();
        Debug::RELEASESUCCESS("RenderingEngine");
    }

	void RenderingEngine::recreateSwapChain()
	{
		int width = 0, height = 0;
		glfwGetFramebufferSize(m_vulkanDeviceMisc.str_window, &width, &height);
		while (width == 0 || height == 0)
		{
			glfwGetFramebufferSize(m_vulkanDeviceMisc.str_window, &width, &height);
			glfwWaitEvents();
		}
		m_ptrClass->settingManager->setWindowHeight(height);
		m_ptrClass->settingManager->setWindowWidth(width);        
		vkDeviceWaitIdle(m_vulkanDeviceMisc.str_device);
		RenderingEngine::m_commandBuffer.release();
		RenderingEngine::m_frameBuffers.release();
		RenderingEngine::m_depthResources.release();
		RenderingEngine::m_colorResources.release();
		m_swapChain.release();
		m_swapChain.initialize(&m_vulkanMisc, m_ptrClass, &m_shaderUniformBufferDivers);
		RenderingEngine::m_colorResources.initialize(&m_vulkanMisc);
		RenderingEngine::m_depthResources.initialize(&m_vulkanMisc);
		RenderingEngine::m_frameBuffers.initialize(&m_vulkanMisc);
		RenderingEngine::m_commandBuffer.initialize(&m_shadowManager,&m_vulkanMisc, m_ptrClass);        
		m_hud.recreateSwapChain();
        m_cameraManager.getCurrentCamera()->mapMemory();
	}

    void RenderingEngine::drawFrame()
    {		
		m_cameraManager.updateFlyCam();
		m_shaderUniformBufferDivers.updateUniformBufferDiver();
		if (m_VulkanDescriptor.recreateCommandBuffer)
		{
			vkDeviceWaitIdle(m_vulkanDeviceMisc.str_device);            
			m_modelManager.destroyElement();
			m_lightManager.destroyElement();
			m_swapChain.recreatePipeline();
			RenderingEngine::m_commandBuffer.release();  
            if (m_VulkanDescriptor.recreateShadowPipeline)
            {
                m_shadowManager.recreatePipeline();
                m_VulkanDescriptor.recreateShadowPipeline = false;
            }
			RenderingEngine::m_commandBuffer.initialize(&m_shadowManager,&m_vulkanMisc, m_ptrClass);
			m_VulkanDescriptor.recreateCommandBuffer = false;            
		}		
		vkWaitForFences(m_vulkanDeviceMisc.str_device, 1, &m_syncObjects.m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);
		
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(m_vulkanDeviceMisc.str_device, m_vulkanSwapChainMisc.str_swapChain, UINT64_MAX, m_syncObjects.m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RenderingEngine::recreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			Debug::Error("Echec l'or de l'optention d'une image de la swap chain");
		}		
				
		if (m_syncObjects.m_imagesInFlight[imageIndex] != VK_NULL_HANDLE)
		{
			vkWaitForFences(m_vulkanDeviceMisc.str_device, 1, &m_syncObjects.m_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
		}
		m_syncObjects.m_imagesInFlight[imageIndex] = m_syncObjects.m_inFlightFences[m_currentFrame];

		m_hud.render(imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_syncObjects.m_imageAvailableSemaphores[m_currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		
		std::array<VkCommandBuffer, 2> submitCommandBuffers =
		{ m_VulkanCommandeBufferMisc.str_commandBuffers[imageIndex], m_hud.m_imGuiCommandBuffers[imageIndex] };

		submitInfo.commandBufferCount = static_cast<uint32_t>(submitCommandBuffers.size());
		submitInfo.pCommandBuffers = submitCommandBuffers.data();

		VkSemaphore signalSemaphores[] = { m_VulkanSynchronisation.str_renderFinishedSemaphores[m_currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(m_vulkanDeviceMisc.str_device, 1, &m_syncObjects.m_inFlightFences[m_currentFrame]);

		if (vkQueueSubmit(m_vulkanDeviceMisc.str_graphicsQueue, 1, &submitInfo, m_syncObjects.m_inFlightFences[m_currentFrame]) != VK_SUCCESS)
		{
			Debug::Error("Echec l'or de la soumission au command buffer");
		}
		
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_vulkanSwapChainMisc.str_swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(m_windowSurface.getPresentQueue(), &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window.framebufferResized)
		{
			m_window.framebufferResized = false;
			RenderingEngine::recreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			Debug::Error("Echec l'or de la presentation");
		}
		m_currentFrame = (m_currentFrame + 1) % m_VulkanSynchronisation.MAX_FRAMES_IN_FLIGHT;	
    }

    VulkanMisc *RenderingEngine::getVulkanMisc()
    {
        return &m_vulkanMisc;
    }
}