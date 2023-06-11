#ifndef __ENGINE_SHADOW_MATRIX__
#define __ENGINE_SHADOW_MATRIX__

#include "glm/glm.hpp"

struct ShadowMatrix
{
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3 pos;
};

#endif//!__ENGINE_SHADOW_MATRIX__

