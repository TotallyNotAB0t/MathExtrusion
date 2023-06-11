#include "SpotLight.hpp"

namespace Ge
{
	SpotLight::SpotLight(int index, VulkanMisc * vM) : Lights(index, vM)
	{
		m_ubl.status = 2;
	}
}