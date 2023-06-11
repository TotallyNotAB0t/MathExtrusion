#ifndef __ENGINE_POINTEUR_CLASS__
#define __ENGINE_POINTEUR_CLASS__

#include "SettingManager.hpp"
#include "InputManager.hpp"
#include "Time.hpp"
#include "ModelManager.hpp"
#include "TextureManager.hpp"
#include "LightManager.hpp"
#include "CameraManager.hpp"
#include "MaterialManager.hpp"
#include "Hud.hpp"
#include "BehaviourManager.hpp"
#include "SceneManager.hpp"
#include "GraphiquePipelineManager.hpp"
#include "SkyboxManager.hpp"
#include "PhysicsEngine.hpp"
#include "ShadowManager.hpp"

using namespace Ge;

struct ptrClass
{
	SettingManager * settingManager;
    InputManager * inputManager;	
	Time * time;
	PhysicsEngine* physicsEngine;
	ModelManager * modelManager;
	TextureManager * textureManager;
	LightManager * lightManager;
	CameraManager * cameraManager;
	MaterialManager * materialManager;
	BehaviourManager * behaviourManager;
	SceneManager * sceneManager;
	SkyboxManager * skyboxManager;
	Hud * hud;	
	GraphiquePipelineManager * graphiquePipelineManager;
	ShadowManager* shadowManager;
};

#endif //__ENGINE_POINTEUR_CLASS__