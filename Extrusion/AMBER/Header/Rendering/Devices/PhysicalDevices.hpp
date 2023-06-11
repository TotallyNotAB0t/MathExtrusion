#ifndef __ENGINE_PHYSICAL_DEVICES__
#define __ENGINE_PHYSICAL_DEVICES__

#include "ValidationLayer.hpp"
#include "InstanceVulkan.hpp"
#include "WindowSurface.hpp"
#include "QueueFamilyIndices.hpp"

namespace Ge
{
    class PhysicalDevices
    {
    public:
        static VkSampleCountFlagBits getMaxUsableSampleCount(VulkanMisc *vM);
        static bool isDeviceSuitable(VkPhysicalDevice device, VulkanMisc *vM);
        static bool checkDeviceExtensionSupport(VkPhysicalDevice device, VulkanMisc *vM);
        static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VulkanMisc *vM);
        static std::vector<const char *> getDeviceExtensions();
        static uint32_t getCountQueueFamily(VkPhysicalDevice device);
        static uint32_t getGraphiqueIndexFamily(VkPhysicalDevice device);

    public:
        bool initialize(VulkanMisc *vM);
        void release();

    private:
        uint32_t m_deviceCount = 0;
        std::vector<VkPhysicalDevice> m_devices;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT; //multi sampling pour �viter le cr�nelage
        VkPhysicalDeviceProperties m_physicalDeviceProperties;
        static std::vector<const char *> deviceExtensions;
    };
}

#endif //__ENGINE_PHYSICAL_DEVICES__