#include "GameEngine.hpp"

namespace Ge
{
	ptrClass GameEngine::m_pointeurClass;
	GameEngine::GameEngine()
	{
		m_pointeurClass.settingManager = &m_settingManager;
		m_pointeurClass.inputManager = &m_inputManager;
		m_pointeurClass.time = &m_time;
        m_pointeurClass.physicsEngine = &m_physicsEngine;
		m_pointeurClass.behaviourManager = &m_behaviourManager;
		m_pointeurClass.sceneManager = &m_sceneManager;
	}

	ptrClass GameEngine::getPtrClass()
	{
		return m_pointeurClass;
	}

    bool GameEngine::initialize()
    {
        if(!m_renderingEngine.initialize(&m_pointeurClass))
        {
            Debug::INITFAILED("RenderingEngine");
            return false;
        }        
        m_VulkanMisc = m_renderingEngine.getVulkanMisc();
        if(!m_inputManager.initialize(m_VulkanMisc))
        {
            Debug::INITFAILED("InputManager");
            return false;
        }
        return true;
    }

    void GameEngine::release()
    {        
		m_renderingEngine.release();
        m_time.release();
        m_inputManager.release();
        m_physicsEngine.Shutdown();
        Debug::RELEASESUCCESS("GameEngine");
    }

    void GameEngine::start()
    {        
        m_time.startTime();
        m_physicsEngine.Initialize(m_settingManager.getGravity());
        Debug::Info("Moteur Start");
		m_sceneManager.loadEntryScene();
        GameEngine::update();
    }
    
    void GameEngine::update()
    {
        while (!glfwWindowShouldClose(m_VulkanMisc->str_VulkanDeviceMisc->str_window))/*gestion d'evenement lier a la fermeture de la fenetre via la croix */
		{
			glfwPollEvents();/*event de recuperation*/
            m_time.fixedUpdateTime();        
            m_physicsEngine.Update(m_time.getFixedDeltaTime());
			m_lag += m_time.getFixedDeltaTime();
			m_behaviourManager.fixedUpdate();
			if (m_lag >= 1.0/m_settingManager.getFps())
			{
				m_time.updateTime();                
				m_inputManager.updateAxis();
				m_behaviourManager.update();
				m_renderingEngine.drawFrame();
				m_lag -= 1.0/m_settingManager.getFps();
			}
		}
    }
}