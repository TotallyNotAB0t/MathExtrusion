#include "ExtrusionScene.hpp"

void ExtrusionScene::load()
{
	m_pc = GameEngine::getPtrClass();
	ShapeBuffer* sb = m_pc.modelManager->allocateBuffer("../Model/plane.obj");
	Model* plane = m_pc.modelManager->createModel(sb, "Plane");
	m_em = new ExtrusionManager(plane);
	m_pc.behaviourManager->addBehaviour(m_em);
	m_pc.hud->addBlockUI(m_em);
	
	
	Textures* t = m_pc.textureManager->createTexture("../Texture/damier.png", false);
	Materials* mat = m_pc.materialManager->createMaterial();
	mat->setMetallic(0.8f);
	mat->setRoughness(0.2f);
	mat->setAlbedoTexture(t);
	mat->setMetallicTexture(t);
	mat->setTilling(glm::vec2(5.0f,5.0f));
	plane->setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
	plane->setScale(glm::vec3(10.0f,1.0f,10.0f));
	plane->setMaterial(mat);

	m_pc.lightManager->createPointLight(glm::vec3(10.0f, 5.0f, -10.0f), glm::vec3(1.0f));
	m_pc.lightManager->createPointLight(glm::vec3(-10.0f, 5.0f, 10.0f), glm::vec3(1.0f));
	m_pc.lightManager->createPointLight(glm::vec3(-10.0f, 5.0f, -10.0f), glm::vec3(1.0f));
	m_pc.lightManager->createPointLight(glm::vec3(10.0f, 5.0f, 10.0f), glm::vec3(1.0f));
}

void ExtrusionScene::unload()
{
	m_pc.behaviourManager->removeBehaviour(m_em);
	m_pc.hud->removeBlockUI(m_em);
}