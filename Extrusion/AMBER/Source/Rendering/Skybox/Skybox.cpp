#include "Skybox.hpp"

namespace Ge
{
	Skybox::Skybox(TextureCubeMap * tCM, ShapeBuffer * m, int indexPipeline)
	{
		m_textureCubeMap = tCM;
		m_buffer = m;
		m_indexPipeline = indexPipeline;
	}

	int Skybox::getIndexPipeline()
	{
		return m_indexPipeline;
	}

	void Skybox::render(VkCommandBuffer CmdBuffer, GraphiquePipeline * pipeline, std::vector<VkDescriptorSet> descriptorSets, VkShaderStageFlags pushConstantShaderFlags, VmaBuffer instanceBuffer)
	{
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindPipeline(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->m_graphiquePipelineElement.m_graphicsPipeline);
		vkCmdBindDescriptorSets(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->m_graphiquePipelineElement.m_pipelineLayout, 0, static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(), 0, nullptr);

		vkCmdBindVertexBuffers(CmdBuffer, 0, 1, m_buffer->getVertexBuffer(), offsets);
		vkCmdBindVertexBuffers(CmdBuffer, 1, 1, &(instanceBuffer.buffer), offsets);

		vkCmdBindIndexBuffer(CmdBuffer, m_buffer->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

		vkCmdDrawIndexed(CmdBuffer, m_buffer->getIndiceSize(), 1, 0, 0, 0);
	}

	TextureCubeMap * Skybox::getTextureCubeMap()
	{
		return m_textureCubeMap;
	}
}