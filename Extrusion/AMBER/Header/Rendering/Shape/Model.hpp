#ifndef __ENGINE_MODEL_SHAPE__
#define __ENGINE_MODEL_SHAPE__

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp" 
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/common.hpp"
#include "Debug.hpp"
#include "VulkanMisc.hpp"
#include "ShapeBuffer.hpp"
#include "BufferManager.hpp"
#include "UniformBufferObject.hpp"
#include "PushConstants.hpp"
#include "Materials.hpp"
#include "GObject.hpp"

namespace Ge
{
	class Model : public GObject
	{
	public:
		Model(ShapeBuffer * buffer, int indexUbo, VulkanMisc * vM);
		~Model();		
		VkBuffer getUniformBuffers();
		ShapeBuffer * getShapeBuffer();
		UniformBufferObject getUBO();
		void setMaterial(Materials * m);
		Materials * getMaterial();
		PushConstants getPushConstants();
		void majMaterialIndex(int pi_mat);
		void mapMemory() override;
		void setIndexUbo(int index);
	private:
		VulkanMisc * vulkanM;
		ShapeBuffer * m_buffer;
		Materials * m_material;
		VkDevice m_device;
		VmaBuffer m_vmaUniformBuffer;
		PushConstants m_index{};
		UniformBufferObject m_ubo{};
	};
}

#endif // __ENGINE_MODEL_SHAPE__
