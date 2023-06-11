#include "MaterialManager.hpp"
#include "ModelManager.hpp"

namespace Ge
{
	Materials * MaterialManager::defaultMaterial = nullptr;
    bool MaterialManager::initialize(VulkanMisc *vM)
    {
        vulkanM = vM;
		defaultMaterial = createMaterial();
        Debug::INITSUCCESS("MaterialManager");
        return true;
    }

	Materials * MaterialManager::getDefaultMaterial()
	{
		return defaultMaterial;
	}

    Materials * MaterialManager::createMaterial()
    {     
		Materials * material = new Materials(m_materials.size(), vulkanM);		
		m_materials.push_back(material);
		vulkanM->str_VulkanDescriptor->materialCount = m_materials.size();
        updateDescriptor();
		return material;
    }

    void MaterialManager::destroyMaterial(Materials *material)
    {
		m_materials.erase(std::remove(m_materials.begin(), m_materials.end(), material), m_materials.end());
		int mat_pi = material->getPipelineIndex();
        delete(material);
		for (int i = 0; i < m_materials.size(); i++)
		{
			m_materials[i]->setIndex(i);
		}
		std::vector<Model *> all_models = ModelManager::GetModels();
		for (int i = 0; i < all_models.size();i++)
		{
			all_models[i]->majMaterialIndex(mat_pi);
		}
        vulkanM->str_VulkanDescriptor->materialCount = m_materials.size();
        updateDescriptor();
		vulkanM->str_VulkanDescriptor->recreateCommandBuffer = true;
    }

    void MaterialManager::updateDescriptor()
    {
        std::vector<VkDescriptorBufferInfo> bufferInfoMaterial{};
		VkDescriptorBufferInfo bufferIM{};
		for (int i = 0; i < m_materials.size(); i++)
		{
            bufferIM.buffer = m_materials[i]->getUniformBuffers();
			bufferIM.offset = 0;
			bufferIM.range = sizeof(UniformBufferMaterial);
			bufferInfoMaterial.push_back(bufferIM);
        }
        m_descriptor[0]->updateCount(vulkanM,m_materials.size(),bufferInfoMaterial);
    }

    void MaterialManager::release()
    {
        for (int i = 0; i < m_materials.size();i++)
		{
			delete (m_materials[i]);
		}
		m_materials.clear();
		for (int i = 0; i < m_descriptor.size(); i++)
		{
			delete m_descriptor[i];
		}
		m_descriptor.clear();
        Debug::RELEASESUCCESS("MaterialManager");
    }

	void MaterialManager::initDescriptor(VulkanMisc * vM)
	{
		if (m_descriptor.size() == 0)
		{
			m_descriptor.push_back(new Descriptor(vM, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1));
		}
	}

}