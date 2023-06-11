#ifndef __ENGINE_UNIFORM_BUFFER_LIGHT__
#define __ENGINE_UNIFORM_BUFFER_LIGHT__

#include "glm/glm.hpp"

struct UniformBufferLight
{
    glm::vec3 position;
    alignas(16) glm::vec3 color;
	alignas(16) glm::vec3 direction;
	float range;
	float spotAngle;
	int status; //DirLight = 0 ; PointLight = 1 ; SpotLight = 2
};

#endif //__ENGINE_UNIFORM_BUFFER_LIGHT__