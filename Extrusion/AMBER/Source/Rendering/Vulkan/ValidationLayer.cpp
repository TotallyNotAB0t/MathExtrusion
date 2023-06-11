#include "ValidationLayer.hpp"

namespace Ge
{
	ValidationLayer * ValidationLayer::s_pInstance = nullptr;
    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

	bool ValidationLayer::initialize(VulkanMisc * vM)
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo(createInfo);
        p_vM = vM;
		if (CreateDebugUtilsMessengerEXT(vM->str_VulkanDeviceMisc->str_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)
		{
			//Debug::Error("Echec de la configuration du ValidationLayer");
		}
		Debug::INITSUCCESS("ValidationLayer");
		return true;
	}

	void ValidationLayer::release()
	{
		if (enableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(p_vM->str_VulkanDeviceMisc->str_instance, m_debugMessenger, nullptr);
		}

		Debug::RELEASESUCCESS("ValidationLayer");
	}

	const std::vector<const char*> ValidationLayer::getValidationLayers()
	{
		return Ge::validationLayers;
	}

	std::vector<const char*> ValidationLayer::getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (ValidationLayer::enableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool ValidationLayer::getEnableValidationLayers()
	{
		return ValidationLayer::enableValidationLayers;
	}

	VkResult ValidationLayer::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void ValidationLayer::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}

	bool ValidationLayer::checkValidationLayerSupport()
	{
		if (!ValidationLayer::s_pInstance)
		{
			Debug::Error("Le ValidationLayer n'est pas initialiser");
			return false;
		}
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		bool layerFound = false;

		for (const char* layerName : ValidationLayer::s_pInstance->validationLayers)
		{
			layerFound = false;
			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}
		}

		return true;
	}

	void ValidationLayer::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT; //VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL ValidationLayer::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		Debug::VLayer("%s",pCallbackData->pMessage);// message de calback du driver
		return VK_FALSE;
	}
}