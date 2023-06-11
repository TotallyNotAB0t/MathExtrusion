#ifndef __ENGINE_RENDERING_ENGINE__
#define __ENGINE_RENDERING_ENGINE__

#include "Debug.hpp"
#include "PointeurClass.hpp"
#include "InstanceVulkan.hpp"
#include "ValidationLayer.hpp"
#include "Window.hpp"
#include "WindowSurface.hpp"
#include "PhysicalDevices.hpp"
#include "LogicalDevice.hpp"
#include "CommandPool.hpp"
#include "BufferManager.hpp"
#include "SwapChain.hpp"
#include "ColorResources.hpp"
#include "DepthResources.hpp"
#include "FrameBuffers.hpp"
#include "TextureManager.hpp"
#include "ModelManager.hpp"
#include "MaterialManager.hpp"
#include "ShaderUniformBufferDivers.hpp"
#include "LightManager.hpp"
#include "CameraManager.hpp"
#include "CommandBuffer.hpp"
#include "Hud.hpp"
#include "SyncObjects.hpp"
#include "SkyboxManager.hpp"
#include "ShadowManager.hpp"

namespace Ge
{
    class RenderingEngine
    {
    public:
        RenderingEngine();
        bool initialize(ptrClass * p_ptrClass);
        void release();
        void drawFrame();
		void recreateSwapChain();
        VulkanMisc * getVulkanMisc();
    private:
        VulkanMisc m_vulkanMisc;
        VulkanDeviceMisc m_vulkanDeviceMisc;
        VulkanSwapChainMisc m_vulkanSwapChainMisc;
        VulkanDescriptor m_VulkanDescriptor;
        VulkanCommandeBufferMisc m_VulkanCommandeBufferMisc;
		VulkanSynchronisation m_VulkanSynchronisation;
		ptrClass * m_ptrClass;

        Window m_window;
        ValidationLayer m_validationLayer;
		InstanceVulkan m_instanceVulkan;
        WindowSurface m_windowSurface;
        PhysicalDevices m_physicalDevice;
        LogicalDevice m_logicalDevice;
        CommandPool m_commandPool;
        BufferManager m_bufferManager;
        SwapChain m_swapChain;
        ColorResources m_colorResources;
        DepthResources m_depthResources;
        FrameBuffers m_frameBuffers;
        TextureManager m_textureManager;
        ModelManager m_modelManager;
        MaterialManager m_materialManager;
        ShaderUniformBufferDivers m_shaderUniformBufferDivers;
        LightManager m_lightManager;
        CameraManager m_cameraManager;
        CommandBuffer m_commandBuffer;
		Hud m_hud;
		SyncObjects m_syncObjects;
		SkyboxManager m_skyboxManager;
        ShadowManager m_shadowManager;
		size_t m_currentFrame = 0;
    };
}

#endif //__ENGINE_RENDERING_ENGINE__