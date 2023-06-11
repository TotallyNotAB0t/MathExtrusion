#ifndef __ENGINE_INSTANCE_VULKAN__
#define __ENGINE_INSTANCE_VULKAN__

#include "vulkan/vulkan.h"
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "ValidationLayer.hpp"
#include "Debug.hpp"
#include "VulkanMisc.hpp"
#include "Version.hpp"

namespace Ge
{
    class InstanceVulkan
    {
    public:
        friend class RenderingEngine;
        bool initialize(const char *name, Version v, VulkanMisc *vM);
        void release();

    private:
        VkInstance instance;
        static std::vector<const char *> instanceExtensions;
    };
}
#endif //__ENGINE_INSTANCE_VULKAN__