#include "ShapeBuffer.hpp"

namespace Ge
{
	ShapeBuffer::ShapeBuffer(std::vector<Vertex> vertices, std::vector<uint32_t> indices, VulkanMisc * vM)
	{
		m_vertices = vertices;
		m_indices = indices;
		device = vM->str_VulkanDeviceMisc->str_device;
		m_bufferSize = sizeof(uint32_t) * indices.size();
		m_indiceSize = static_cast<uint32_t>(m_indices.size());

		VmaBuffer stagingBufferI;
		BufferManager::createBuffer(m_bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBufferI, vM->str_VulkanDeviceMisc);
		memcpy(BufferManager::mapMemory(stagingBufferI), indices.data(), (size_t)m_bufferSize);
		BufferManager::unMapMemory(stagingBufferI);
		BufferManager::createBuffer(m_bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vmaIndexBuffer, vM->str_VulkanDeviceMisc);
		BufferManager::copyBuffer(stagingBufferI.buffer, m_vmaIndexBuffer.buffer, m_bufferSize, vM);
		BufferManager::destroyBuffer(stagingBufferI);

		m_bufferSize = sizeof(Vertex) * vertices.size();

		VmaBuffer stagingBufferV;
		BufferManager::createBuffer(m_bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBufferV, vM->str_VulkanDeviceMisc);

		memcpy(BufferManager::mapMemory(stagingBufferV), m_vertices.data(), (size_t)m_bufferSize);
		BufferManager::unMapMemory(stagingBufferV);

		BufferManager::createBuffer(m_bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vmaVertexBuffer, vM->str_VulkanDeviceMisc);
		BufferManager::copyBuffer(stagingBufferV.buffer, m_vmaVertexBuffer.buffer, m_bufferSize, vM);
		BufferManager::destroyBuffer(stagingBufferV);
	}

	ShapeBuffer::~ShapeBuffer()
	{
		BufferManager::destroyBuffer(m_vmaIndexBuffer);
		BufferManager::destroyBuffer(m_vmaVertexBuffer);
	}

	VkBuffer ShapeBuffer::getIndexBuffer()
	{
		return m_vmaIndexBuffer.buffer;
	}

	VkBuffer * ShapeBuffer::getVertexBuffer()
	{
		return &m_vmaVertexBuffer.buffer;
	}

	VkDeviceSize ShapeBuffer::getDeviceSize()
	{
		return m_bufferSize;
	}

	uint32_t ShapeBuffer::getIndiceSize()
	{
		return m_indiceSize;
	}

	std::vector<Vertex> ShapeBuffer::getVertices()
	{
		return m_vertices;
	}
}