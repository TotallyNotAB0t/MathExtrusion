#ifndef __ENGINE_SCENE_MANAGER__
#define __ENGINE_SCENE_MANAGER__

#include "Scene.hpp"
#include <map>
#include <string>

namespace Ge
{
	class SceneManager
	{
	public:
		void addScene(std::string name, Scene * s);
		void removeScene(std::string name);
		void loadScene(std::string name);
		void loadEntryScene();
	private:
		std::string entryScene = "";
		std::map<std::string,Scene *> m_mapScene;
		Scene * currentScene;
	};
}

#endif//__ENGINE_SCENE_MANAGER__