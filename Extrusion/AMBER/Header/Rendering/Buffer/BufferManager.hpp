#ifndef __ENGINE_BUFFER_MANAGER__
#define __ENGINE_BUFFER_MANAGER__

#include "Debug.hpp"
#include "VmaBufferImage.hpp"
#include "VmaBuffer.hpp"
#include "VulkanMisc.hpp"

namespace Ge
{
    class BufferManager
    {
    private:
        friend class RenderingEngine;
        bool initialize(VulkanMisc * vM);
        void release();

    public:
        static bool createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags flags, VmaBuffer &buffer, VulkanDeviceMisc *vM);
        static bool createImageBuffer(uint32_t width, uint32_t height, VkImageType imageType, uint32_t arrayLayers, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VmaBufferImage& image, VkImageCreateFlags flags, VulkanMisc * vM, VkImageLayout initiallayout = VK_IMAGE_LAYOUT_UNDEFINED);
        static void destroyBuffer(VmaBuffer &buffer);
        static void destroyImageBuffer(VmaBufferImage &buffer);
        static void *mapMemory(VmaBuffer &buffer);
        static void unMapMemory(VmaBuffer &buffer);
        static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VulkanMisc *vM);
        static void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t baseArrayLayer, uint32_t bufferOffset, VulkanMisc * vM);
        static VkCommandBuffer beginSingleTimeCommands(VulkanMisc *vM);
        static void endSingleTimeCommands(VkCommandBuffer commandBuffer, VulkanMisc *vM);

    private:
        static BufferManager *s_pInstance;
        VmaAllocator m_allocator;
    };
}

#endif //__ENGINE_BUFFER_MANAGER__