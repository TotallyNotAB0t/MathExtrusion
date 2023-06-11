#include "GraphiquePipeline.hpp"

namespace Ge
{
	GraphiquePipeline::GraphiquePipeline(VulkanMisc *vM, ShaderPair * sp)
	{
		vulkanM = vM;
		m_shaderPair = sp;
		ShaderElement VertShader = LoadShader(sp->Vert, "main", vM->str_VulkanDeviceMisc->str_device, true,vM);
		ShaderElement FragShader = LoadShader(sp->Frag, "main", vM->str_VulkanDeviceMisc->str_device, false,vM);
		std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages;
		shaderStages[0] = VertShader.shaderStageCreateInfo;
		shaderStages[1] = FragShader.shaderStageCreateInfo;

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		std::vector<VkVertexInputBindingDescription> bindingDescription = { Vertex::getBindingDescription(), PushConstants::getBindingDescription()};
		std::array<VkVertexInputAttributeDescription, 7> attributeDescriptions;
		std::array<VkVertexInputAttributeDescription, 5> arr1 = Vertex::getAttributeDescriptions();
		std::array<VkVertexInputAttributeDescription, 2> arr2 = PushConstants::getAttributeDescriptions(5);

		std::copy(arr1.begin(), arr1.end(), attributeDescriptions.begin());
		std::copy(arr2.begin(), arr2.end(), attributeDescriptions.begin() + arr1.size());

		vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescription.size());
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = bindingDescription.data();
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)vM->str_VulkanSwapChainMisc->str_swapChainExtent.width;
		viewport.height = (float)vM->str_VulkanSwapChainMisc->str_swapChainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = {0, 0};
		scissor.extent = vM->str_VulkanSwapChainMisc->str_swapChainExtent;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL; // VK_POLYGON_MODE_FILL; VK_POLYGON_MODE_LINE;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = m_shaderPair->multiSampling ? VK_TRUE : VK_FALSE; // Activation du sample shading dans la pipeline
		multisampling.rasterizationSamples = vM->str_VulkanDeviceMisc->str_msaaSamples;
		multisampling.minSampleShading = .2f; // Fraction minimale pour le sample shading; plus proche de 1 lisse d'autant plus

		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = m_shaderPair->back ? VK_FALSE : VK_TRUE;
		depthStencil.depthWriteEnable = m_shaderPair->back ? VK_FALSE : VK_TRUE;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PushConstants);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(vM->str_VulkanSwapChainMisc->str_descriptorSetLayout.size());
		pipelineLayoutInfo.pSetLayouts = vM->str_VulkanSwapChainMisc->str_descriptorSetLayout.data();
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		pipelineLayoutInfo.pushConstantRangeCount = 1;

		if (vkCreatePipelineLayout(vM->str_VulkanDeviceMisc->str_device, &pipelineLayoutInfo, nullptr, &m_graphiquePipelineElement.m_pipelineLayout) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation d'un pipeline layout");
		}

		VkPipelineCacheCreateInfo cacheInfo{};
		cacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		cacheInfo.pNext = NULL;
		cacheInfo.flags = 0;
		cacheInfo.initialDataSize = 0;
		cacheInfo.pInitialData = nullptr;

		if (vkCreatePipelineCache(vM->str_VulkanDeviceMisc->str_device, &cacheInfo, nullptr, &m_graphiquePipelineElement.m_graphicsPipelineCache) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation d'un cache pour le pipeline");
		}

		std::vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.pDynamicStates = dynamicStateEnables.data();
		dynamicState.dynamicStateCount = dynamicStateEnables.size();
		dynamicState.flags = 0;

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = shaderStages.size();
		pipelineInfo.pStages = shaderStages.data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencil;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = m_graphiquePipelineElement.m_pipelineLayout;
		pipelineInfo.renderPass = vM->str_VulkanSwapChainMisc->str_renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.pNext = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines(vM->str_VulkanDeviceMisc->str_device, m_graphiquePipelineElement.m_graphicsPipelineCache, 1, &pipelineInfo, nullptr, &m_graphiquePipelineElement.m_graphicsPipeline) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation du pipeline graphique");
		}

		DestroyShaderElement(vM->str_VulkanDeviceMisc->str_device, VertShader);
		DestroyShaderElement(vM->str_VulkanDeviceMisc->str_device, FragShader);
		m_index = (int)(vM->str_VulkanSwapChainMisc->str_graphiquePipelineElement.size());
		vM->str_VulkanSwapChainMisc->str_graphiquePipelineElement.push_back(&m_graphiquePipelineElement);		
	}

	int GraphiquePipeline::getIndex()
	{		
		return m_index;
	}

	ShaderPair * GraphiquePipeline::getShaderPair()
	{
		return m_shaderPair;
	}

	GraphiquePipeline::~GraphiquePipeline()
	{
		vulkanM->str_VulkanSwapChainMisc->str_graphiquePipelineElement.erase(std::remove(vulkanM->str_VulkanSwapChainMisc->str_graphiquePipelineElement.begin(), vulkanM->str_VulkanSwapChainMisc->str_graphiquePipelineElement.end(), &m_graphiquePipelineElement), vulkanM->str_VulkanSwapChainMisc->str_graphiquePipelineElement.end());
		vkDestroyPipelineCache(vulkanM->str_VulkanDeviceMisc->str_device, m_graphiquePipelineElement.m_graphicsPipelineCache, nullptr);
		vkDestroyPipeline(vulkanM->str_VulkanDeviceMisc->str_device, m_graphiquePipelineElement.m_graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(vulkanM->str_VulkanDeviceMisc->str_device, m_graphiquePipelineElement.m_pipelineLayout, nullptr);
	}

	ShaderElement GraphiquePipeline::LoadShader(const std::string &filename, const char *entry, VkDevice device, bool isVertex, VulkanMisc *vM)
	{
		ShaderElement se;
		std::vector<char> ShaderCode;
		if (vM->str_VulkanSwapChainMisc->str_shaderLoader.count(filename) > 0)
		{
			ShaderCode = vM->str_VulkanSwapChainMisc->str_shaderLoader[filename];
		}
		else
		{
			ShaderCode = readFile(filename);
		}
		VkShaderModule ShaderModule = createShaderModule(ShaderCode, device);

		VkPipelineShaderStageCreateInfo ShaderStageInfo{};
		ShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		ShaderStageInfo.stage = isVertex ? VK_SHADER_STAGE_VERTEX_BIT : VK_SHADER_STAGE_FRAGMENT_BIT;
		ShaderStageInfo.module = ShaderModule;
		ShaderStageInfo.pName = entry;

		se.shaderModule = ShaderModule;
		se.shaderStageCreateInfo = ShaderStageInfo;
		return se;
	}

	void GraphiquePipeline::DestroyShaderElement(VkDevice device, ShaderElement se)
	{
		vkDestroyShaderModule(device, se.shaderModule, nullptr);
	}

	std::vector<char> GraphiquePipeline::readFile(const std::string &filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if (!file.is_open())
		{
			Debug::Error("Echec de l'ouverture du fichier %s", filename.c_str());
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	VkShaderModule GraphiquePipeline::createShaderModule(const std::vector<char> &code, VkDevice device)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pNext = VK_NULL_HANDLE;
		createInfo.flags = 0;
		createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation d'un shader module");
		}

		return shaderModule;
	}
}