#ifndef __ENGINE_SHADER_ELEMENT__
#define __ENGINE_SHADER_ELEMENT__

#include "vulkan/vulkan.h"

struct ShaderElement
{
    VkShaderModule shaderModule;
    VkPipelineShaderStageCreateInfo shaderStageCreateInfo;
};

#endif //__ENGINE_SHADER_ELEMENT__