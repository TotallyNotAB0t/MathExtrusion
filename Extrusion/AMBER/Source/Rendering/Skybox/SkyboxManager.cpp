#include "SkyboxManager.hpp"
#include "TextureManager.hpp"

namespace Ge
{
	Skybox * SkyboxManager::currentSkybox = nullptr;
	bool SkyboxManager::initialize(VulkanMisc * vM, ModelManager * mM, GraphiquePipelineManager * gPM)
	{
		vulkanM = vM;
		currentSkybox = nullptr;
		float pos[72] = { -5,-5,-5,5,5,-5,-5,5,-5,5,-5,-5,-5,-5,5,5,5,5,5,-5,5,-5,5,5,-5,-5,-5,5,-5,5,5,-5,-5,-5,-5,5,5,-5,-5,5,5,5,5,5,-5,5,-5,5,5,5,-5,-5,5,5,-5,5,-5,5,5,5,-5,5,-5,-5,-5,5,-5,-5,-5,-5,5,5 };
		float texCord[48] = { 1,1,0,0,1,0,0,1,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0 };
		float normal[72] = { 0,-0,1,0,-0,1,0,-0,1,0,-0,1,0,0,-1,0,0,-1,0,0,-1,0,0,-1,-1,-0,0,1,0,-0,1,0,-0,-1,-0,0,0,-1,0,0,1,-0,0,1,-0,0,-1,0,1,0,-0,-1,-0,0,-1,-0,0,1,0,-0,0,1,-0,0,-1,0,0,-1,0,0,1,-0 };
		unsigned int indice[36] = { 0,1,2,1,0,3,4,5,6,5,4,7,8,9,10,9,8,11,12,13,14,13,12,15,16,17,18,17,16,19,20,21,22,21,20,23 };
		cubeMapBaseModel = mM->allocateBuffer(pos, texCord, texCord, indice,24,36);
		m_skyboxPipeline = gPM->createPipeline("../Shader/skybox_fs.spv","../Shader/skybox_vs.spv",true);
		loadSkybox(TextureManager::GetNullCubeMap());
		currentSkybox = nullptr;
		Debug::INITSUCCESS("SkyboxManager");
		return true;
	}

	Skybox * SkyboxManager::GetCurrentSkybox()
	{
		return currentSkybox;
	}

	void SkyboxManager::initDescriptor(VulkanMisc * vM)
	{
		if (m_descriptor.size() == 0)
		{
			m_descriptor.push_back(new Descriptor(vM, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1));
		}
	}

	void SkyboxManager::updateDescriptor()
	{
		std::vector<VkDescriptorImageInfo> imageInfo{};
		VkDescriptorImageInfo imageI{};
		for (int i = 0; i < 1; i++)
		{
			imageI.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageI.imageView = currentSkybox->getTextureCubeMap()->getVkImageView();
			imageI.sampler = currentSkybox->getTextureCubeMap()->getVkSampler();
			imageInfo.push_back(imageI);
		}
		m_descriptor[0]->updateCount(vulkanM, 1, imageInfo);
	}

	Skybox * SkyboxManager::loadSkybox(TextureCubeMap * tCM)
	{		
		if (currentSkybox != nullptr)
		{
			m_skybox.erase(std::find(m_skybox.begin(), m_skybox.end(), currentSkybox), m_skybox.end());
			delete(currentSkybox);
		}
		currentSkybox = new Skybox(tCM, cubeMapBaseModel, m_skyboxPipeline->getIndex());
		m_skybox.push_back(currentSkybox);
		updateDescriptor();
		return currentSkybox;
	}

	Skybox* SkyboxManager::createSkybox(TextureCubeMap* tCM)
	{
		return new Skybox(tCM, cubeMapBaseModel, m_skyboxPipeline->getIndex());
	}
	
	void SkyboxManager::changeSkybox(Skybox* sky)
	{
		if (sky == nullptr)
		{
			currentSkybox = m_skybox[0];
		}
		else
		{
			currentSkybox = sky;
		}
		updateDescriptor();
	}

	void SkyboxManager::release()
	{
		currentSkybox = nullptr;
		for (int i = 0; i < m_skybox.size(); i++)
		{
			delete(m_skybox[i]);
		}
		m_skybox.clear();
		for (int i = 0; i < m_descriptor.size(); i++)
		{
			delete m_descriptor[i];
		}
		m_descriptor.clear();
		Debug::RELEASESUCCESS("SkyboxManager");
	}
}