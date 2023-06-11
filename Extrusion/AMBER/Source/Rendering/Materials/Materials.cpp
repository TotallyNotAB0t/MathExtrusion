#include "Materials.hpp"

namespace Ge
{
	Materials::Materials(int index, VulkanMisc * vM)
	{
		vulkanM = vM;
		m_ubm.albedo = glm::vec3(1.0f, 1.0f, 1.0f);
		m_color[0] = m_ubm.albedo.x;
		m_color[1] = m_ubm.albedo.y;
		m_color[2] = m_ubm.albedo.z;
		m_ubm.metallic = 1.0f;		
		m_ubm.roughness = 1.0f;
		m_ubm.normal = 1.0f;
		m_ubm.ao = 1.0f;
		m_ubm.albedoMap = 0;
		m_ubm.normalMap = 1;
		m_ubm.metallicMap = 0;
		m_ubm.roughnessMap = 0;
		m_ubm.aoMap = 0;				
		m_ubm.castShadow = true;
		m_ubm.tilling = glm::vec2(1.0f);
		m_ubm.offset = glm::vec2(0.0f);
		m_offset[0] = m_ubm.offset.x;
		m_offset[1] = m_ubm.offset.y;
		m_tilling[0] = m_ubm.tilling.x;
		m_tilling[1] = m_ubm.tilling.y;

		if (!BufferManager::createBuffer(sizeof(UniformBufferMaterial), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_vmaUniformBuffer, vM->str_VulkanDeviceMisc))
		{
			Debug::Error("Echec de la creation d'un uniform buffer object");
		}
		m_pipelineIndex = 0;
		m_index = index;
		updateUniformBufferMaterial();
	}

	void Materials::setColor(glm::vec3 color)
	{
		m_ubm.albedo = color;
		updateUniformBufferMaterial();
	}

	void Materials::setMetallic(float metal)
	{
		m_ubm.metallic = metal;
		updateUniformBufferMaterial();
	}

	void Materials::setRoughness(float roug)
	{
		m_ubm.roughness = roug;
		updateUniformBufferMaterial();
	}

	void Materials::setNormal(float normal)
	{
		m_ubm.normal = normal;
		updateUniformBufferMaterial();
	}

	void Materials::setOclusion(float ao)
	{
		m_ubm.ao = ao;
		updateUniformBufferMaterial();
	}

	void Materials::setPipeline(GraphiquePipeline * p)
	{
		m_pipelineIndex = p->getIndex();
	}

	int Materials::getPipelineIndex()
	{
		return m_pipelineIndex;
	}

	void Materials::setAlbedoTexture(Textures * albedo)
	{
		m_ubm.albedoMap = albedo->getIndex();
		m_albedoMap = albedo;
		updateUniformBufferMaterial();
	}

	void Materials::setNormalTexture(Textures * normal)
	{
		m_ubm.normalMap = normal->getIndex();
		m_normalMap = normal;
		updateUniformBufferMaterial();
	}

	void Materials::setMetallicTexture(Textures * metal)
	{
		m_ubm.metallicMap = metal->getIndex();
		m_metallicMap = metal;
		updateUniformBufferMaterial();
	}

	void Materials::setRoughnessTexture(Textures * roug)
	{
		m_ubm.roughnessMap = roug->getIndex();
		m_RoughnessMap = roug;
		updateUniformBufferMaterial();
	}

	void Materials::setOclusionTexture(Textures * oclu)
	{
		m_ubm.aoMap = oclu->getIndex();
		m_aoMap = oclu;
		updateUniformBufferMaterial();
	}

	glm::vec3 Materials::getColor()
	{
		return m_ubm.albedo;
	}

	float Materials::getMetallic()
	{
		return m_ubm.metallic;
	}

	float Materials::getRoughness()
	{
		return m_ubm.roughness;
	}

	float Materials::getNormal()
	{
		return m_ubm.normal;
	}

	float Materials::getOclusion()
	{
		return m_ubm.ao;
	}

	Textures * Materials::getAlbedoTexture()
	{
		return m_albedoMap;
	}

	Textures * Materials::getNormalTexture()
	{
		return m_normalMap;
	}

	Textures * Materials::getMetallicTexture()
	{
		return m_metallicMap;
	}

	Textures * Materials::getRoughnessTexture()
	{
		return m_RoughnessMap;
	}

	Textures * Materials::getOclusionTexture()
	{
		return m_aoMap;
	}

	VkBuffer Materials::getUniformBuffers()
	{
		return m_vmaUniformBuffer.buffer;
	}

	void Materials::updateUniformBufferMaterial()
	{
		memcpy(BufferManager::mapMemory(m_vmaUniformBuffer), &m_ubm, sizeof(UniformBufferMaterial));
		BufferManager::unMapMemory(m_vmaUniformBuffer);
	}

	int Materials::getIndex()
	{
		return m_index;
	}
	void Materials::setIndex(int i)
	{
		m_index = i;
	}

	void Materials::majTextureIndex()
	{
		if (m_albedoMap)
		{
			m_ubm.albedoMap = m_albedoMap->getIndex();
		}
		if (m_normalMap)
		{
			m_ubm.normalMap = m_normalMap->getIndex();
		}
		if (m_metallicMap)
		{
			m_ubm.metallicMap = m_metallicMap->getIndex();
		}
		if (m_RoughnessMap)
		{
			m_ubm.roughnessMap = m_RoughnessMap->getIndex();
		}
		if (m_aoMap)
		{
			m_ubm.aoMap = m_aoMap->getIndex();
		}
		updateUniformBufferMaterial();
	}

	bool Materials::getShadowCast()
	{
		return m_ubm.castShadow;
	}

	void Materials::setShadowCast(bool state)
	{
		m_ubm.castShadow = state;
		updateUniformBufferMaterial();
	}

	glm::vec2 Materials::getOffset()
	{
		return m_ubm.offset;
	}

	glm::vec2 Materials::getTilling()
	{
		return m_ubm.tilling;
	}

	void Materials::setOffset(glm::vec2 off)
	{
		m_ubm.offset = off;
		updateUniformBufferMaterial();
	}

	void Materials::setTilling(glm::vec2 tilling)
	{
		m_ubm.tilling = tilling;
		updateUniformBufferMaterial();
	}

	void Materials::onGUI()
	{
		ImGui::TextColored(ImVec4(0.2f, 1, 0.2f, 1), "Material\n");
		if (ImGui::ColorEdit3("Albedo", (float *)&m_ubm.albedo))
		{
			updateUniformBufferMaterial();
		}
		if (ImGui::DragFloat2("Tilling", (float*)&m_ubm.tilling, 0.2f))
		{
			updateUniformBufferMaterial();
		}
		if (ImGui::DragFloat2("Offset", (float *)&m_ubm.offset, 0.2f))
		{
			updateUniformBufferMaterial();
		}
		if (ImGui::DragFloat("Metallic", &m_ubm.metallic, 0.05f))
		{
			updateUniformBufferMaterial();
		}
		if (ImGui::DragFloat("Roughness", &m_ubm.roughness, 0.05f))
		{
			updateUniformBufferMaterial();
		}
		if (ImGui::DragFloat("Normal", &m_ubm.normal, 0.05f))
		{
			updateUniformBufferMaterial();
		}
		if (ImGui::DragFloat("AO", &m_ubm.ao, 0.05f))
		{
			updateUniformBufferMaterial();
		}
	}

	Materials::~Materials()
	{
		BufferManager::destroyBuffer(m_vmaUniformBuffer);
	}
}