#ifndef __ENGINE_HIEARCHY__
#define __ENGINE_HIEARCHY__

#include "ImguiBlock.hpp"
#include "GObject.hpp"

namespace Ge
{
	class Hiearchy : public ImguiBlock
	{
	public:
		Hiearchy();
		void preRender(VulkanMisc* vM);
		void render(VulkanMisc* vM);
	private:		
		ImGuiWindowFlags window_flags;
		int m_listboxCurrentItem = 0;
		bool m_open = true;
	};
}


#endif //__ENGINE_HIEARCHY__