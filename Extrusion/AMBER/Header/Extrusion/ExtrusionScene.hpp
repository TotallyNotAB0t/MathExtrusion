#ifndef __EXTRUSION_SCENE__
#define __EXTRUSION_SCENE__

#include "Scene.hpp"
#include "GameEngine.hpp"
#include <vector>
#include "ExtrusionManager.hpp"

class ExtrusionScene : public Scene
{
public:
	void load();
	void unload();
private:
	ptrClass m_pc;
	ExtrusionManager m_em;
};

#endif //!__EXTRUSION_SCENE__
