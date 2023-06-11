#ifndef __LIGHT_DATA__
#define __LIGHT_DATA__

#include "Transform.hpp"
#include "UniformBufferLight.hpp"

struct LightData
{
	Transform* transform;
	UniformBufferLight* ubl;
};


#endif //!__LIGHT_DATA__