#ifndef __ENGINE_BEHAVIOUR__
#define __ENGINE_BEHAVIOUR__

#include "Component.hpp"

class Behaviour : public Component
{
public:
	virtual void start() = 0;
	virtual void fixedUpdate() = 0;
	virtual void update() = 0;
	virtual void stop() = 0;
};

#endif//__ENGINE_BEHAVIOUR__