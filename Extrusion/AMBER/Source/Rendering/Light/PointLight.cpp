#include "PointLight.hpp"

namespace Ge
{
	PointLight::PointLight(int index, VulkanMisc * vM) : Lights(index,vM)
	{
		m_ubl.status = 1;
	}
}