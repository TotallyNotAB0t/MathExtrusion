#include "CommandBuffer.hpp"
#include "GraphiquePipelineManager.hpp"
#include "ModelManager.hpp"
#include  "Descriptor.hpp"
#include "SkyboxManager.hpp"

namespace Ge
{
	bool CommandBuffer::initialize(ShadowManager * shadowManager,VulkanMisc * vM, ptrClass * ptrC)
	{
		vulkanM = vM;
		std::vector<VkFramebuffer> swapChainFramebuffers = vM->str_VulkanCommandeBufferMisc->str_swapChainFramebuffers;		
		std::vector<VkDescriptorSet> tab_Descriptor;
		std::vector<Descriptor *> all_descriptor = Descriptor::GetAllDescriptor();
		std::map<ShapeBuffer*, std::map<int, std::vector<Model*>>> models_instanting = ModelManager::GetModelInstancing();
		std::vector<std::vector<Model*>> separateByPipeline;

		for (const auto& pair : models_instanting) 
		{			
			for (const auto& splitBy : pair.second)
			{
				separateByPipeline.push_back(splitBy.second);
			}
		}
	
		m_instancedBuffer.reserve(separateByPipeline.size());
		int countInstance = 0;
		for (const auto& vec : separateByPipeline)
		{			
			m_instancedBuffer.push_back(createInstanceBuffer(vec));
		}

		BufferManager::createBuffer(sizeof(PushConstants), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_instancedBufferSkybox, vulkanM->str_VulkanDeviceMisc);

		for (const auto& descriptor : all_descriptor) 
		{
			tab_Descriptor.push_back(descriptor->getDescriptorSets());
		}
		
		m_commandBuffers.resize(swapChainFramebuffers.size());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = vM->str_VulkanCommandeBufferMisc->str_commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

		if (vkAllocateCommandBuffers(vM->str_VulkanDeviceMisc->str_device, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
		{
			Debug::Error("Echec de l'allocation du command buffers");
			return false;
		}
		std::vector<Shadow*> shadows = shadowManager->getShadows();
		std::vector<std::vector<VkFramebuffer>> currentShadowFrames;
		ShapeBuffer* shapeB;
		VkDeviceSize offsets[] = { 0 };
		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { ptrC->settingManager->getClearColor().r, ptrC->settingManager->getClearColor().g, ptrC->settingManager->getClearColor().b, ptrC->settingManager->getClearColor().a };
		clearValues[1].depthStencil = { 1.0f, 0 };
		for (size_t i = 0; i < m_commandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;			
			std::vector<std::vector<Model*>> all_models = separateByPipeline;
			std::vector<VmaBuffer> instancedBuffer = m_instancedBuffer;
			std::vector<GraphiquePipeline*> all_pipeline = GraphiquePipelineManager::GetPipelines();
			if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS)
			{
				Debug::Error("Echec de l'enregistrer du commande buffer");
				return false;
			}

			VkViewport viewport{};
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkOffset2D offset{};
			offset.x = 0;
			offset.y = 0;
			VkRect2D scissor{};
			scissor.offset = offset;

			VkRenderPassBeginInfo renderPassInfoShadow{};
			renderPassInfoShadow.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfoShadow.renderPass = shadowManager->getRenderPass();			
			renderPassInfoShadow.renderArea.offset = offset;
			renderPassInfoShadow.renderArea.extent.height = TEXTURE_DIM;
			renderPassInfoShadow.renderArea.extent.width = TEXTURE_DIM;

			renderPassInfoShadow.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfoShadow.pClearValues = clearValues.data();

			viewport.width = TEXTURE_DIM;
			viewport.height = TEXTURE_DIM;
			scissor.extent.width = viewport.width;
			scissor.extent.height = viewport.height;

			vkCmdSetViewport(m_commandBuffers[i], 0, 1, &viewport);
			vkCmdSetScissor(m_commandBuffers[i], 0, 1, &scissor);
			int globalShadowFrameCount = 0;
			std::array<VkDescriptorSet, 2> shadowDesciptor { vM->str_VulkanSwapChainMisc->str_descriptorSetModel , (shadowManager->getDescriptor()[1])->getDescriptorSets() };
			for (int j = 0; j < shadows.size(); j++)
			{		
				currentShadowFrames = shadows[j]->getFrameBuffer();
				for (int k = 0; k < currentShadowFrames.size(); k++)
				{
					renderPassInfoShadow.framebuffer = currentShadowFrames[k][i];
					vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfoShadow, VK_SUBPASS_CONTENTS_INLINE);					
					vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, shadowManager->getGraphiquePipelineElement().m_graphicsPipeline);
					PushConstantShadow pcs{k};

					for (int m = 0; m < all_models.size(); m++)
					{
						vkCmdBindDescriptorSets(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, shadowManager->getGraphiquePipelineElement().m_pipelineLayout, 0, shadowDesciptor.size(), shadowDesciptor.data(), 0, nullptr);
						shapeB = all_models[m][0]->getShapeBuffer();
						vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, shapeB->getVertexBuffer(), offsets);
						vkCmdBindVertexBuffers(m_commandBuffers[i], 1, 1, &(instancedBuffer[m].buffer), offsets);

						vkCmdBindIndexBuffer(m_commandBuffers[i], shapeB->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
												
						vkCmdPushConstants(m_commandBuffers[i], shadowManager->getGraphiquePipelineElement().m_pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstantShadow), &pcs);

						vkCmdDrawIndexed(m_commandBuffers[i], shapeB->getIndiceSize(), all_models[m].size(), 0, 0, 0);
					}
					globalShadowFrameCount++;

					vkCmdEndRenderPass(m_commandBuffers[i]);
				}
			}			

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = vM->str_VulkanSwapChainMisc->str_renderPass;
			renderPassInfo.framebuffer = swapChainFramebuffers[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = vM->str_VulkanSwapChainMisc->str_swapChainExtent;

			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { ptrC->settingManager->getClearColor().r, ptrC->settingManager->getClearColor().g, ptrC->settingManager->getClearColor().b, ptrC->settingManager->getClearColor().a };
			clearValues[1].depthStencil = { 1.0f, 0 };			

			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			viewport.width = vM->str_VulkanSwapChainMisc->str_swapChainExtent.width;
			viewport.height = vM->str_VulkanSwapChainMisc->str_swapChainExtent.height;
			scissor.extent.width = viewport.width;
			scissor.extent.height = viewport.height;

			vkCmdSetViewport(m_commandBuffers[i], 0, 1, &viewport);
			vkCmdSetScissor(m_commandBuffers[i], 0, 1, &scissor);

			vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			Skybox * sky = SkyboxManager::GetCurrentSkybox();
			if (sky != nullptr)
			{
				for (int p = 0; p < all_pipeline.size(); p++)
				{
					if (sky->getIndexPipeline() == all_pipeline[p]->getIndex())
					{
						SkyboxManager::GetCurrentSkybox()->render(m_commandBuffers[i], all_pipeline[p], tab_Descriptor, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, m_instancedBufferSkybox);
						all_pipeline.erase(all_pipeline.begin() + p);
						p = all_pipeline.size();
					}
				}
			}
			
			for (int p = 0; p < all_pipeline.size(); p++)
			{
				vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, all_pipeline[p]->m_graphiquePipelineElement.m_graphicsPipeline);				
				for (int m = 0; m < all_models.size(); m++)
				{
					if (all_models[m][0]->getMaterial()->getPipelineIndex() == all_pipeline[p]->getIndex())
					{		
						vkCmdBindDescriptorSets(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, all_pipeline[p]->m_graphiquePipelineElement.m_pipelineLayout, 0, static_cast<uint32_t>(tab_Descriptor.size()), tab_Descriptor.data(), 0, nullptr);
						shapeB = all_models[m][0]->getShapeBuffer();
						vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, shapeB->getVertexBuffer(), offsets);
						vkCmdBindVertexBuffers(m_commandBuffers[i], 1, 1, &(instancedBuffer[m].buffer), offsets);

						vkCmdBindIndexBuffer(m_commandBuffers[i], shapeB->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

						vkCmdDrawIndexed(m_commandBuffers[i], shapeB->getIndiceSize(), all_models[m].size(), 0, 0, 0);

						instancedBuffer.erase(instancedBuffer.begin() + m);
						all_models.erase(all_models.begin()+ m);
						if(all_models.size() > 0)
						{
							m--;
						}
					}
				}
			}

			vkCmdEndRenderPass(m_commandBuffers[i]);

			if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS)
			{
				Debug::Error("Echec de l'enregistrer du commande buffer");
				return false;
			}
		}
		
		vM->str_VulkanCommandeBufferMisc->str_commandBuffers = m_commandBuffers;

		Debug::INITSUCCESS("CommandBuffers");
		return true;
	}

	VmaBuffer CommandBuffer::createInstanceBuffer(std::vector<Model*> models)
	{
		VmaBuffer stagingBufferI;
		VmaBuffer m_vmaInstanceBuffer;
		size_t bufferSize = models.size() * sizeof(PushConstants);
		PushConstants * pushList = new PushConstants[models.size()];
		for (int i = 0; i < models.size(); i++)
		{
			pushList[i] = models[i]->getPushConstants();
		}
		BufferManager::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBufferI, vulkanM->str_VulkanDeviceMisc);
		memcpy(BufferManager::mapMemory(stagingBufferI), pushList, bufferSize);
		BufferManager::unMapMemory(stagingBufferI);
		BufferManager::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vmaInstanceBuffer, vulkanM->str_VulkanDeviceMisc);
		BufferManager::copyBuffer(stagingBufferI.buffer, m_vmaInstanceBuffer.buffer, bufferSize, vulkanM);
		BufferManager::destroyBuffer(stagingBufferI);
		delete[] pushList;
		return m_vmaInstanceBuffer;
	}
	
	void CommandBuffer::release()
	{
		for (int i = 0; i < m_instancedBuffer.size(); i++)
		{
			BufferManager::destroyBuffer(m_instancedBuffer[i]);
		}		
		m_instancedBuffer.clear();
		BufferManager::destroyBuffer(m_instancedBufferSkybox);
		vkFreeCommandBuffers(vulkanM->str_VulkanDeviceMisc->str_device, vulkanM->str_VulkanCommandeBufferMisc->str_commandPool, static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());		
		Debug::RELEASESUCCESS("CommandBuffers");
	}
}