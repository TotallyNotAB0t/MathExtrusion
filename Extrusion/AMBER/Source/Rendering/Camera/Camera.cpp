#include "Camera.hpp"
#include "CameraManager.hpp"

namespace Ge
{
	Camera::Camera(VulkanMisc * vM) : GObject()
	{
		if (!BufferManager::createBuffer(sizeof(UniformBufferCamera), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_vmaUniformBuffer, vM->str_VulkanDeviceMisc)) 
		{
			Debug::Error("Echec de la creation d'un uniform buffer object");
		}
		m_far = 500.0f;
		m_fov = 70.0f;
		m_near = 0.1f;
		m_priority = 0;
		vulkanM = vM;
		m_ortho = false;
		m_orthoSize = 10.0f;
		mapMemory();
	}	

	Camera::~Camera()
	{		
		BufferManager::destroyBuffer(m_vmaUniformBuffer);
	}

	float Camera::aspectRatio() const
	{
		return (float)vulkanM->str_VulkanSwapChainMisc->str_swapChainExtent.width / (float)vulkanM->str_VulkanSwapChainMisc->str_swapChainExtent.height;
	}

	glm::mat4 Camera::getViewMatrix() const
	{
		return glm::inverse(getModelMatrix());
	}

	glm::mat4 Camera::getProjectionMatrix() const
	{
		glm::mat4 projectionMatrix;
		if (m_ortho) 
		{
			float halfHeight = m_orthoSize * 0.5f;
			float halfWidth = halfHeight * aspectRatio();
			projectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, m_near, m_far);
		}
		else
		{
			projectionMatrix = glm::perspective(glm::radians(m_fov), aspectRatio(), m_near, m_far);
		}

		projectionMatrix = glm::scale(projectionMatrix, glm::vec3(1.0f, -1.0f, 1.0f));
		//projectionMatrix[1][1] *= -1;

		return projectionMatrix;
	}

	void Camera::mapMemory()
	{		
		m_uniformBufferCamera.camPos = m_transform.position;
		m_uniformBufferCamera.view = getViewMatrix();
		m_uniformBufferCamera.proj = getProjectionMatrix();
		memcpy(BufferManager::mapMemory(m_vmaUniformBuffer), &m_uniformBufferCamera, sizeof(UniformBufferCamera));
		BufferManager::unMapMemory(m_vmaUniformBuffer);
	}

	VkBuffer Camera::getUniformBuffer()
	{
		return m_vmaUniformBuffer.buffer;
	}

	void Camera::setFieldOfView(float fov)
	{
		m_fov = fov;
		mapMemory();
	}

	void Camera::setNear(float n)
	{
		m_near = n;
		mapMemory();
	}

	void Camera::setFar(float f)
	{
		m_far = f;
		mapMemory();
	}

	void Camera::setPriority(int p)
	{
		m_priority = p;
		mapMemory();
	}

	void Camera::setOrtho(bool state)
	{
		m_ortho = state;
		mapMemory();
	}

	float Camera::getFieldOfView()
	{
		return m_fov;
	}

	float Camera::getNear()
	{
		return m_near;
	}

	float Camera::getFar()
	{
		return m_far;
	}

	int Camera::getPriority()
	{
		return m_priority;
	}

	void Camera::onGUI()
	{
		GObject::onGUI();
		ImGui::TextColored(ImVec4(0.2f, 1, 0.2f, 1), "Camera\n");
		if (ImGui::DragFloat("Fov", &m_fov,2.0f,10.0f,180.0f))
		{
			mapMemory();
		}
		if (ImGui::DragFloat("Near", &m_near, 0.5f, 0.0001f, 10.0f))
		{
			mapMemory();
		}
		if (ImGui::DragFloat("Far", &m_far, 0.5f, 1.0f, 10000.0f))
		{
			mapMemory();
		}
		if (ImGui::DragInt("Priority", &m_priority, 1.0f))
		{
			mapMemory();
		}
		if (ImGui::Checkbox("Ortho", &m_ortho))
		{
			mapMemory();
		}
		if (ImGui::DragFloat("OrthoSize", &m_orthoSize, 1.0f))
		{
			mapMemory();
		}
	}
}