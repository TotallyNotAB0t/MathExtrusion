
#include <iostream>
#include "GameEngine.hpp"
#include "ExtrusionScene.hpp"
using namespace Ge;

int main()
{
	Debug::Info("Moteur Graphique");	
	GameEngine engine;	
	ptrClass pc = engine.getPtrClass();	
	pc.settingManager->setName("Extrusion");
	pc.settingManager->setClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	pc.settingManager->setWindowHeight(991);
	pc.settingManager->setWindowWidth(1920);

	ExtrusionScene es;
	
	pc.sceneManager->addScene("ExtrusionScene", &es);

	if (!engine.initialize())
	{
		Debug::Error("Erreur d'intialisation du moteur graphique");
		return -1;
	}		

	try
	{
		engine.start();
	}
	catch (std::runtime_error& e)
	{
		Debug::Error("Exception : %s", e.what());
		return -1;
	}
	catch (std::bad_alloc& e)
	{
		Debug::Error("Exception : %s", e.what());
		return -1;
	}
	catch (const std::exception& e)
	{
		Debug::Error("Exception : %s", e.what());
		return -1;
	}
	engine.release();
	std::cin.ignore();
	return 0;
}

