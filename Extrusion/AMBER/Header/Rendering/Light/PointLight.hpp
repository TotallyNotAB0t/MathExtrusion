#ifndef __ENGINE_POINT_LIGHT__
#define __ENGINE_POINT_LIGHT__

#include "Lights.hpp"

namespace Ge
{
	class PointLight : public Lights
	{
	public:
		PointLight(int index, VulkanMisc * vM);
	};
}

#endif //__ENGINE_POINT_LIGHT__