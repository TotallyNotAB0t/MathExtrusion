#ifndef __ENGINE_DIRECTIONAL_LIGHT__
#define __ENGINE_DIRECTIONAL_LIGHT__

#include "Lights.hpp"

namespace Ge
{
	class DirectionalLight : public Lights
	{
	public:
		DirectionalLight(int index, VulkanMisc * vM);

	};
}

#endif //__ENGINE_DIRECTIONAL_LIGHT__