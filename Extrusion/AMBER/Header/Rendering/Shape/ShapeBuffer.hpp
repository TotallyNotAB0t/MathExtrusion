#ifndef __ENGINE_MODEL_BUFFER__
#define __ENGINE_MODEL_BUFFER__

#include "VulkanMisc.hpp"
#include "BufferManager.hpp"
#include "VertexVulkan.hpp"
#include <memory.h>

namespace Ge
{
	class ShapeBuffer
	{
	public:
		ShapeBuffer(std::vector<Vertex> vertices, std::vector<uint32_t> indices, VulkanMisc * vM);
		~ShapeBuffer();
		VkBuffer getIndexBuffer();
		VkBuffer * getVertexBuffer();
		VkDeviceSize getDeviceSize();		
		std::vector<Vertex> getVertices();
		uint32_t getIndiceSize();
	private:
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
		VkDevice device;
		VmaBuffer m_vmaIndexBuffer;
		VmaBuffer m_vmaVertexBuffer;
		VkDeviceSize m_bufferSize;
		uint32_t m_indiceSize;	
	};
}

#endif //__ENGINE_MODEL_BUFFER__
