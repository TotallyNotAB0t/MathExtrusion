#ifndef __ENGINE_UNIFORM_BUFFER_CAMERA__
#define __ENGINE_UNIFORM_BUFFER_CAMERA__

#include "glm/glm.hpp"

struct UniformBufferCamera
{
	alignas(16) glm::vec3 camPos;
    alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

#endif //__ENGINE_UNIFORM_BUFFER_CAMERA__