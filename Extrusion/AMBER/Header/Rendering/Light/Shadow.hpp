#ifndef __SHADOW__
#define __SHADOW__

#include "Debug.hpp"
#include "VulkanMisc.hpp"
#include "BufferManager.hpp"
#include "GObject.hpp"
#include "ShadowMatrix.hpp"
#define TEXTURE_DIM 1024

namespace Ge
{
	class Shadow
	{
	public:
		Shadow(VkImageView depth,VkRenderPass renderPass, Transform * lightTransform, VulkanMisc* vM);
		~Shadow();
		std::vector<VkBuffer> getUniformBuffers();
		std::vector<VkImageView> getImageView();
		VkSampler getImageSampler();
		std::vector<std::vector<VkFramebuffer>> getFrameBuffer();
		void createFrameBuffer(VkImageView depth, VkRenderPass renderPass);
		float aspectRatio() const;
		void UpdateUniformBuffer(int frame);
		void mapMemory();
	private:
		VulkanMisc* vMisc;
		VmaBufferImage m_depthTexture;
		VkSampler m_textureSampler;
		std::vector<VmaBuffer> m_vmaUniformBuffers;
		std::vector<VkImageView> m_depthTextureView;
		std::vector<ShadowMatrix> m_pushConstantShadow;
		Transform* m_lightTransform;
		std::vector< std::vector<VkFramebuffer>> m_frameBuffer;
	};
}

#endif //!__SHADOW__