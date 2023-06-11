#ifndef __ENGINE_TRANSFORM__
#define __ENGINE_TRANSFORM__

#include "glm/glm.hpp"

struct Transform
{
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
};

#endif //__ENGINE_TRANSFORM__