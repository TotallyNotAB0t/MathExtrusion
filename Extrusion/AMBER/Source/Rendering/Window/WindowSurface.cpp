#include "WindowSurface.hpp"

namespace Ge
{
    bool WindowSurface::initialize(VulkanMisc * vM)
    {
        vulkanM = vM;
        if(glfwCreateWindowSurface(vM->str_VulkanDeviceMisc->str_instance, vM->str_VulkanDeviceMisc->str_window, nullptr, &m_surface) != VK_SUCCESS)// creation de la la VkSurfaceKHR pour faire le rendu sur la fenetre via la swap chain
		{
			Debug::Error("Echec de la creation de la window surface");
			return false;
		}
		vM->str_VulkanDeviceMisc->str_surface = m_surface;
        Debug::INITSUCCESS("WindowSurface");
        return true;
    }

    void WindowSurface::release()
    {
        vkDestroySurfaceKHR(vulkanM->str_VulkanDeviceMisc->str_instance, m_surface, nullptr);// destruction la window surface
		vulkanM->str_VulkanDeviceMisc->str_surface = VK_NULL_HANDLE;
        Debug::RELEASESUCCESS("WindowSurface");
    }

    void WindowSurface::SetupPresentQueue(unsigned int value, VulkanMisc * vM)
    {
        vkGetDeviceQueue(vM->str_VulkanDeviceMisc->str_device, value, 0, &m_presentQueue);
        vM->str_VulkanDeviceMisc->str_presentQueue = m_presentQueue;
    }

	VkQueue WindowSurface::getPresentQueue()
	{
		return m_presentQueue;
	}
}