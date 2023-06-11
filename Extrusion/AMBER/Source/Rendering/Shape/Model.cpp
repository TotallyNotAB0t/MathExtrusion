#include "Model.hpp"
#include "ModelManager.hpp"

namespace Ge
{
	Model::Model(ShapeBuffer * buffer, int indexubo, VulkanMisc * vM) : GObject()
	{
		vulkanM = vM;
		m_buffer = buffer;
		m_device = vM->str_VulkanDeviceMisc->str_device;

		if (!BufferManager::createBuffer(sizeof(UniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_vmaUniformBuffer, vM->str_VulkanDeviceMisc)) 
		{
			Debug::Error("Echec de la creation d'un uniform buffer object");
		}
		m_index.ubo = indexubo;
		m_index.material = 0;
		m_material = nullptr;
		mapMemory();
	}

	Model::~Model()
	{
		BufferManager::destroyBuffer(m_vmaUniformBuffer);
	}

	void Model::mapMemory()
	{
		m_ubo.model = getModelMatrix();		
		memcpy(BufferManager::mapMemory(m_vmaUniformBuffer), &m_ubo, sizeof(m_ubo));
		BufferManager::unMapMemory(m_vmaUniformBuffer);
	}

	void Model::setMaterial(Materials * m)
	{			
		int pi = 0;
		if (m_material != nullptr)
		{
			pi = m_material->getPipelineIndex();			
			removeComponent((Component *)m_material);
		}
		addComponent((Component *)m);
		ModelManager::updateInstanced(m_buffer, this, pi, m->getPipelineIndex(), vulkanM);
		m_material = m;				
		m_index.material = m_material->getIndex();	
		vulkanM->str_VulkanDescriptor->recreateCommandBuffer = true;//TODO: verifier si dans ce cas c'est nessaire
	}

	void Model::majMaterialIndex(int pi_mat)
	{
		if (m_material != nullptr)
		{
			m_index.material = m_material->getIndex();
		}
		else
		{
			ModelManager::updateInstanced(m_buffer,this, pi_mat, 0, vulkanM);
			m_index.material = 0;
		}
	}

	Materials * Model::getMaterial()
	{
		return m_material;
	}

	PushConstants Model::getPushConstants()
	{
		return m_index;
	}

	VkBuffer Model::getUniformBuffers()
	{
		return m_vmaUniformBuffer.buffer;
	}

	ShapeBuffer * Model::getShapeBuffer()
	{
		return m_buffer;
	}

	UniformBufferObject Model::getUBO()
	{
		return m_ubo;
	}

	void Model::setIndexUbo(int index)
	{
		m_index.ubo = index;
		vulkanM->str_VulkanDescriptor->recreateCommandBuffer = true;
	}

}
