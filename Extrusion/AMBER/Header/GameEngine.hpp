#ifndef __ENGINE_GAME_ENGINE___
#define __ENGINE_GAME_ENGINE___

#include "Initializer.hpp"
#include "PointeurClass.hpp"
#include "Debug.hpp"
#include "RenderingEngine.hpp"
#include "SettingManager.hpp"
#include "Time.hpp"
#include "InputManager.hpp"
#include "PhysicsEngine.hpp"

namespace Ge
{
    class GameEngine : Initializer
    {
    public:
		GameEngine();
		static ptrClass getPtrClass();
        bool initialize();
        void release();
        void start();
	private:
        void update();
    private:
		static ptrClass m_pointeurClass;
        VulkanMisc * m_VulkanMisc;
        RenderingEngine m_renderingEngine;
        Debug m_debug;
        SettingManager m_settingManager;        
        Time m_time;
        PhysicsEngine m_physicsEngine;
        InputManager m_inputManager;
		BehaviourManager m_behaviourManager;
		SceneManager m_sceneManager;
        float m_lag = 0.0f;
    };
}

#endif //__ENGINE_GAME_ENGINE___