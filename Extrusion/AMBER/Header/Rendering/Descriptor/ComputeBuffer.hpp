#ifndef __COMPUTE_BUFFER__
#define __COMPUTE_BUFFER__

#include "VulkanMisc.hpp"
#include "Debug.hpp"
#include "BufferManager.hpp"

namespace Ge
{
	class ComputeBuffer
	{
	public:
		ComputeBuffer(VulkanMisc* vM,int baseCount,int sizeofR);
		void SetData(void * data, int managedBufferStartIndex, int computeBufferStartIndex, int count);
		void GetData(void * data, int managedBufferStartIndex, int computeBufferStartIndex, int count);
		VkDescriptorSetLayout getDescriptorSetLayout();
		VkDescriptorSet getDescriptorSet();
		~ComputeBuffer();
	private:
		VulkanMisc* vulkanM;
		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkDescriptorPool m_DescriptorPool;
		VkDescriptorSet m_DescriptorSets;
		VkDescriptorType m_DescriptorType;
		VmaBuffer m_vmaBuffer;
		int m_count = 0;
		int m_sizeOfResource = 0;
	};
}

#endif //!__COMPUTE_BUFFER__