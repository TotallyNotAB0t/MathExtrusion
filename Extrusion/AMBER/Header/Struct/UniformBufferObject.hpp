#ifndef __ENGINE_UNIFORM_BUFFER_OBJECTS__
#define __ENGINE_UNIFORM_BUFFER_OBJECTS__

#include "glm/glm.hpp"

struct UniformBufferObject
{
	alignas(16) glm::mat4 model;
};

#endif //__ENGINE_UNIFORM_BUFFER_OBJECTS__