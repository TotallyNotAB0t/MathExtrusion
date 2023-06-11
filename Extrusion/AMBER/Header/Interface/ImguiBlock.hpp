#ifndef __ENGINE_INTERFACE_IMGUI_BLOCK__
#define __ENGINE_INTERFACE_IMGUI_BLOCK__

#define GLFW_INCLUDE_NONE
#include "implot.h"
#include "imgui-cmake/Header/imgui.h"
#include "imgui-cmake/Header/imgui_impl_vulkan.h"
#include "imgui-cmake/Header/imgui_impl_glfw.h"
#include "VulkanMisc.hpp"

namespace Ge
{
	class ImguiBlock
	{
	public:
		virtual void preRender(VulkanMisc* vM) = 0;
		virtual void render(VulkanMisc* vM) = 0;
	};
}

#endif //__ENGINE_INTERFACE_IMGUI_BLOCK__