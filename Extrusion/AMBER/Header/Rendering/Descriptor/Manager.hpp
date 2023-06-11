#ifndef __ENGINE_MANAGER__
#define __ENGINE_MANAGER__

#include "VulkanMisc.hpp"
#include "Descriptor.hpp"

namespace Ge
{
	class Manager
	{
	public:
		virtual void initDescriptor(VulkanMisc * vM) = 0;
		virtual void updateDescriptor() = 0;		
		std::vector<Descriptor*> getDescriptor();
		bool getDestroyElement();
	protected:
		friend class RenderingEngine;
		virtual void destroyElement();
	protected:
		std::vector<Descriptor*> m_descriptor;
		bool m_destroyElement = false;
	};
}

#endif//__ENGINE_MANAGER__