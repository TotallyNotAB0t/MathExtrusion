#ifndef __ENGINE_CAMERA__
#define __ENGINE_CAMERA__

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp" 
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/common.hpp"
#include "UniformBufferCamera.hpp"
#include "VulkanMisc.hpp"
#include "GObject.hpp"
#include "BufferManager.hpp"

namespace Ge
{
	class Camera : public GObject
	{		
	public:
		Camera(VulkanMisc * vM);
		~Camera();
		void setFieldOfView(float fov);
		void setNear(float near);
		void setFar(float far);
		void setPriority(int p);
		void setOrtho(bool state);
		float getFieldOfView();
		float getNear();
		float getFar();
		void mapMemory() override;
		int getPriority();
		float aspectRatio() const;
		glm::mat4 getViewMatrix() const;
		glm::mat4 getProjectionMatrix() const;
		void onGUI() override;
		VkBuffer getUniformBuffer();
	protected:
		VulkanMisc * vulkanM;
		UniformBufferCamera m_uniformBufferCamera;
		VmaBuffer m_vmaUniformBuffer;
		float m_fov = 80.0f;
		float m_near = 0.1f;
		float m_far = 300.0f;
		int m_priority = 0;
		bool m_ortho = false;
		float m_orthoSize;
	};
}

#endif//__ENGINE_CAMERA__