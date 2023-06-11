#ifndef __COMPUTE_SHADER__
#define __COMPUTE_SHADER__

#include "VulkanMisc.hpp"
#include "Debug.hpp"
#include <vector>
#include "ComputeBuffer.hpp"

namespace Ge
{
	class ComputeShader
	{
	public:
		ComputeShader(VulkanMisc* vM, const std::string& shaderPath, const std::vector<ComputeBuffer*>& buffers);
		void dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
		~ComputeShader();
	private:
		static VkShaderModule LoadShader(const std::string& filename, VkDevice device, VulkanMisc* vM);
	private:
		VulkanMisc* vulkanM;
		VkPipeline m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
		std::vector<ComputeBuffer*> m_Buffers;
	};
}

#endif //!__COMPUTE_SHADER__