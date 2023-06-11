#ifndef __ENGINE_PUSH_CONSTANTS__
#define __ENGINE_PUSH_CONSTANTS__

#include "vulkan/vulkan.h"

struct PushConstants//pushContants -> instancing
{
    alignas(int) int ubo;
    alignas(int) int material;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 1;
        bindingDescription.stride = sizeof(PushConstants);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions(int startLocation)
    {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

        attributeDescriptions[0].binding = 1;
        attributeDescriptions[0].location = startLocation;
        attributeDescriptions[0].format = VK_FORMAT_R32_SINT;
        attributeDescriptions[0].offset = offsetof(PushConstants, ubo);

        attributeDescriptions[1].binding = 1;
        attributeDescriptions[1].location = startLocation+1;
        attributeDescriptions[1].format = VK_FORMAT_R32_SINT;
        attributeDescriptions[1].offset = offsetof(PushConstants, material);

        return attributeDescriptions;
    }

    static VkVertexInputAttributeDescription getShadowAttributeDescriptions(int startLocation)
    {
        VkVertexInputAttributeDescription attributeDescriptions{};

        attributeDescriptions.binding = 1;
        attributeDescriptions.location = startLocation;
        attributeDescriptions.format = VK_FORMAT_R32_SINT;
        attributeDescriptions.offset = offsetof(PushConstants, ubo);

        return attributeDescriptions;
    }
};

#endif //__ENGINE_PUSH_CONSTANTS__