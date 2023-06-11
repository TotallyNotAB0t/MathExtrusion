#ifndef __ENGINE_SKYBOX__
#define __ENGINE_SKYBOX__

#include "TextureCubeMap.hpp"
#include "Model.hpp"

namespace Ge
{
	class Skybox
	{
	public:
		Skybox(TextureCubeMap * tCM, ShapeBuffer * m, int indexPipeline);
		TextureCubeMap * getTextureCubeMap();
		void render(VkCommandBuffer CmdBuffer, GraphiquePipeline * pipeline, std::vector<VkDescriptorSet> descriptorSets, VkShaderStageFlags pushConstantShaderFlags,VmaBuffer instanceBuffer);
		int getIndexPipeline();
	private:
		TextureCubeMap * m_textureCubeMap;
		ShapeBuffer * m_buffer;
		PushConstants m_index{};
		int m_indexPipeline;
	};
}

#endif//__ENGINE_SKYBOX__