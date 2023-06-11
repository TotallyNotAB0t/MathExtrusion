#include "Lights.hpp"
#include "ShadowManager.hpp"

namespace Ge
{
	Lights::Lights(int index, VulkanMisc* vM) : GObject()
	{
		vMisc = vM;
		m_index = index;
		m_ubl.color = glm::vec3(1.0f);
		m_ubl.range = 10.0f;
		m_ubl.spotAngle = 45.0f;
		if (!BufferManager::createBuffer(sizeof(UniformBufferLight), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_vmaUniformBuffer, vM->str_VulkanDeviceMisc))
		{
			Debug::Error("Echec de la creation d'un uniform buffer object");
		}		
		m_lightData.transform = &m_transform;
		m_lightData.ubl = &m_ubl;
		mapMemory();
	}

	void Lights::mapMemory()
	{
		m_ubl.position = m_transform.position;
		m_ubl.direction = getDirection();
		updateUniformBufferLight();
		if (m_shadow)
		{
			m_shadowData->mapMemory();
		}
	}

	void Lights::setRange(float r)
	{
		m_ubl.range = r;
		updateUniformBufferLight();
	}

	void Lights::setSpotAngle(float r)
	{
		m_ubl.spotAngle = r;
		updateUniformBufferLight();
	}

	float Lights::getSpotAngle()
	{
		return m_ubl.spotAngle;
	}

	float Lights::getRange()
	{
		return m_ubl.range;
	}

	void Lights::setColors(glm::vec3 color)
	{
		m_ubl.color = color;
		updateUniformBufferLight();
	}

	glm::vec3 Lights::getColors()
	{
		return m_ubl.color;
	}

	int Lights::getStatus()
	{
		return m_ubl.status;
	}

	int Lights::getIndex()
	{
		return m_index;
	}

	void Lights::setIndex(int i)
	{
		m_index = i;
	}

	VkBuffer Lights::getUniformBuffers()
	{
		return m_vmaUniformBuffer.buffer;
	}

	void Lights::setShadow(bool state)
	{
		m_shadow = state;
		if (m_shadow)
		{
			m_shadowData = ShadowManager::getShadowManager()->CreateShadow(&m_lightData);
		}
		else
		{
			ShadowManager::getShadowManager()->RemoveShadow(m_shadowData);
			m_shadowData = nullptr;
		}
	}

	bool Lights::getShadow()
	{
		return m_shadow;
	}

	void Lights::updateUniformBufferLight()
	{
		memcpy(BufferManager::mapMemory(m_vmaUniformBuffer), &m_ubl, sizeof(m_ubl));
		BufferManager::unMapMemory(m_vmaUniformBuffer);
	}

	void Lights::onGUI()
	{
		GObject::onGUI();
		ImGui::TextColored(ImVec4(0.2f, 1, 0.2f, 1), "Light\n");
		if (ImGui::ColorEdit3("Color", (float*)&m_ubl.color))
		{
			setColors(m_ubl.color);
		}

		if (ImGui::DragFloat("Range", &m_ubl.range, 0.2f, 0.01f))
		{
			updateUniformBufferLight();
		}

		if (m_ubl.status == 2)
		{
			if (ImGui::DragFloat("Angle", &m_ubl.spotAngle, 0.2f, 0.01f))
			{
				updateUniformBufferLight();
			}
		}
	}

	Lights::~Lights()
	{
		BufferManager::destroyBuffer(m_vmaUniformBuffer);
	}
}