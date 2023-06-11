#ifndef __SHADOW_MANAGER__
#define __SHADOW_MANAGER__

#include "Shadow.hpp"
#include "LightData.hpp"
#include "Descriptor.hpp"
#include "Manager.hpp"
#include "PushConstantShadow.hpp"

namespace Ge
{
	class ShadowManager : public Manager
	{
	public:
		bool initialize(VulkanMisc* vM);
		void release();
		Shadow * CreateShadow(LightData* light);
		void RemoveShadow(Shadow * shadow);		
		void recreatePipeline();
		void initDescriptor(VulkanMisc* vM);
		void updateDescriptor();
		std::vector<Shadow*> getShadows();
		GraphiquePipelineElement getGraphiquePipelineElement();
		VkRenderPass getRenderPass();
	private:
		bool createPipeline();
		friend class Lights;
		static ShadowManager* getShadowManager();
	private:
		static ShadowManager* s_instance;
		VulkanMisc* vMisc;
		std::vector<Shadow*> m_shadows;
		GraphiquePipelineElement m_graphiquePipelineElement;
		VkRenderPass m_renderPass;	
		VmaBufferImage m_depthTexture;
		VmaBuffer m_vmaUniformBuffers;
		VkImageView m_depthTextureView;
		LightData test{};
		Transform t{};
	};
}


#endif // !__SHADOW_MANAGER__
