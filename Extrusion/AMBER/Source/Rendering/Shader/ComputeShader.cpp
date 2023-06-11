#include "ComputeShader.hpp"
#include "GraphiquePipeline.hpp"
#include "BufferManager.hpp"

namespace Ge
{
	ComputeShader::ComputeShader(VulkanMisc* vM, const std::string& shaderPath, const std::vector<ComputeBuffer*>& buffers) : vulkanM(vM), m_Pipeline(VK_NULL_HANDLE), m_PipelineLayout(VK_NULL_HANDLE), m_Buffers(buffers)
	{
		VkShaderModule computeShader = LoadShader(shaderPath, vM->str_VulkanDeviceMisc->str_device,vM);
		
		VkComputePipelineCreateInfo pipelineCreateInfo{};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pipelineCreateInfo.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		pipelineCreateInfo.stage.module = computeShader;
		pipelineCreateInfo.stage.pName = "main";

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.pushConstantRangeCount = 0;

		std::vector<VkDescriptorSetLayout> layout;
		for (ComputeBuffer* buffer : m_Buffers)
		{
			layout.push_back(buffer->getDescriptorSetLayout());
		}

		pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(layout.size());
		pipelineLayoutCreateInfo.pSetLayouts = layout.data();		

		if(vkCreatePipelineLayout(vulkanM->str_VulkanDeviceMisc->str_device, &pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation d'un pipeline layout");
		}

		pipelineCreateInfo.layout = m_PipelineLayout;

		if(vkCreateComputePipelines(vulkanM->str_VulkanDeviceMisc->str_device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_Pipeline) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation d'un Compute pipeline");
		}

		vkDestroyShaderModule(vulkanM->str_VulkanDeviceMisc->str_device, computeShader, nullptr);
	}

	VkShaderModule ComputeShader::LoadShader(const std::string& filename, VkDevice device, VulkanMisc* vM)
	{
		std::vector<char> ShaderCode;
		if (vM->str_VulkanSwapChainMisc->str_shaderLoader.count(filename) > 0)
		{
			ShaderCode = vM->str_VulkanSwapChainMisc->str_shaderLoader[filename];
		}
		else
		{
			ShaderCode = GraphiquePipeline::readFile(filename);
		}

		return GraphiquePipeline::createShaderModule(ShaderCode, device);
	}

	void ComputeShader::dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
	{
		VkCommandBuffer commandBuffer = BufferManager::beginSingleTimeCommands(vulkanM);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_Pipeline);

		std::vector<VkDescriptorSet> descriptors;
		for (ComputeBuffer* buffer : m_Buffers) {
			descriptors.push_back(buffer->getDescriptorSet());
		}
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_PipelineLayout, 0, static_cast<uint32_t>(descriptors.size()), descriptors.data(), 0, nullptr);

		vkCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);

		BufferManager::endSingleTimeCommands(commandBuffer, vulkanM);
	}

	ComputeShader::~ComputeShader()
	{
		if (m_Pipeline != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(vulkanM->str_VulkanDeviceMisc->str_device, m_Pipeline, nullptr);
			m_Pipeline = VK_NULL_HANDLE;
		}

		if (m_PipelineLayout != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(vulkanM->str_VulkanDeviceMisc->str_device, m_PipelineLayout, nullptr);
			m_PipelineLayout = VK_NULL_HANDLE;
		}
	}
}