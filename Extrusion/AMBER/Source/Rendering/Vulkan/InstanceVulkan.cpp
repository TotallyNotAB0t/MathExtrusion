#include "InstanceVulkan.hpp"

namespace Ge
{
    std::vector<const char *> InstanceVulkan::instanceExtensions =
        {
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME};

    bool InstanceVulkan::initialize(const char *name, Version v, VulkanMisc *vM)
    {
        std::vector<std::string> supportedInstanceExtensions;
        std::vector<const char *> extensions = ValidationLayer::getRequiredExtensions();
        uint32_t extCount = 0;                
        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
        if (extCount > 0)
        {
            std::vector<VkExtensionProperties> extensions(extCount);
            if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
            {
                for (VkExtensionProperties extension : extensions)
                {
                    supportedInstanceExtensions.push_back(extension.extensionName);
                }
            }
        }

        if (instanceExtensions.size() > 0)
        {
            for (const char *enabledExtension : instanceExtensions)
            {
#ifdef _WIN32
                if (std::find(supportedInstanceExtensions.begin(), supportedInstanceExtensions.end(), enabledExtension) == supportedInstanceExtensions.end())
                {
                    std::cerr << "Enabled instance extension \"" << enabledExtension << "\" is not present at instance level\n";
                }
#elif __unix__

                for (int i = 0; i < supportedInstanceExtensions.size(); i++)
                {
                    if (supportedInstanceExtensions[i] == enabledExtension)
                    {
                        i = supportedInstanceExtensions.size();
                    }
                    else if (i + 1 == supportedInstanceExtensions.size())
                    {
                        std::cerr << "Enabled instance extension \"" << enabledExtension << "\" is not present at instance level\n";
                    }
                }

#endif
                extensions.push_back(enabledExtension);
            }
        }

        VkApplicationInfo appInfo{}; //structure de donn�e a envoyer a vulkan
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = name;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "AMBER Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(v.majeur, v.mineur, v.patch);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        const std::vector<const char *> vLayer = ValidationLayer::getValidationLayers();
        for (int i = 0; i < vLayer.size(); i++)
        {
            Debug::Info("Layer : %s", vLayer[i]);
        }

        if (ValidationLayer::getEnableValidationLayers()) // si le mode debug est activer alors on fait la verification des erreur lier au driver
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(vLayer.size()); //envoie les layer pour le debug du driver
            createInfo.ppEnabledLayerNames = vLayer.data();
            ValidationLayer::populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
        }
        else
        {
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            Debug::Error("echec de la creation de l'instance vulkan");
            return false;
        }
        vM->str_VulkanDeviceMisc->str_instance = instance;
        Debug::INITSUCCESS("InstanceVulkan");
        return true;
    }

    void InstanceVulkan::release()
    {
        vkDestroyInstance(instance, nullptr);
        Debug::RELEASESUCCESS("InstanceVulkan");
    }
}