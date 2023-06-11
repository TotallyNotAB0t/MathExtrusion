#include "DirectionalLight.hpp"

namespace Ge
{
	DirectionalLight::DirectionalLight(int index, VulkanMisc * vM) : Lights(index, vM)
	{
		m_ubl.status = 0;
	}
}