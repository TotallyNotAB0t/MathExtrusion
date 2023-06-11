#ifndef __ENGINE_COMMAND_BUFFER__
#define __ENGINE_COMMAND_BUFFER__

#include "VulkanMisc.hpp"
#include "BufferManager.hpp"
#include <array>
#include "Model.hpp"
#include "PointeurClass.hpp"
#include "ShadowManager.hpp"

namespace Ge
{
    class CommandBuffer
    {
    private:
        friend class RenderingEngine;
        bool initialize(ShadowManager* shadowManager,VulkanMisc * vM, ptrClass * ptrC);
        VmaBuffer createInstanceBuffer(std::vector<Model*> models);
        void release();
    private:
        VulkanMisc * vulkanM;
		std::vector<VkCommandBuffer> m_commandBuffers;
        std::vector<VmaBuffer> m_instancedBuffer;
        VmaBuffer m_instancedBufferSkybox;
    };
}

#endif //__ENGINE_COMMAND_BUFFER__