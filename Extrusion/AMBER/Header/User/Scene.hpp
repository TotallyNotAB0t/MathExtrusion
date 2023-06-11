#ifndef __ENGINE_SCENE__
#define __ENGINE_SCENE__

class Scene
{
public:
	virtual void load() = 0;
	virtual void unload() = 0;
};

#endif //__ENGINE_SCENE__