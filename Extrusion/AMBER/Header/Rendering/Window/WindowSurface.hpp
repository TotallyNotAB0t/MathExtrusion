#ifndef __ENGINE_WINDOW_SURFACE__
#define __ENGINE_WINDOW_SURFACE__

#include "Debug.hpp"
#include "VulkanMisc.hpp"

namespace Ge
{
    class WindowSurface
    {
    public:
        bool initialize(VulkanMisc *vM);
        void release();
        void SetupPresentQueue(unsigned int value, VulkanMisc *vM);
		VkQueue getPresentQueue();
    private:		
        VulkanMisc *vulkanM;
        VkQueue m_presentQueue;
        VkSurfaceKHR m_surface;
    };
}

#endif //__ENGINE_WINDOW_SURFACE__