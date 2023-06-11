#ifndef __ENGINE_DESCRIPTOR__
#define __ENGINE_DESCRIPTOR__

#include "VulkanMisc.hpp"
#include "Debug.hpp"
#include <algorithm>

namespace Ge
{
    class Descriptor
    {
    public:
        Descriptor(VulkanMisc* vM, VkDescriptorType descriptorType, int baseCount, bool ignoreDescriptorList = false);
		~Descriptor();
        VkDescriptorSetLayout getDescriptorSetLayout();
        VkDescriptorPool getDescriptorPool();
        VkDescriptorSet getDescriptorSets();
        void updateCount(VulkanMisc *vM, int count, std::vector<VkDescriptorBufferInfo> bufferInfo);
        void updateCount(VulkanMisc *vM, int count, std::vector<VkDescriptorImageInfo> bufferInfo);
		static std::vector<Descriptor *> GetAllDescriptor();
    private:
        static VkDescriptorPool createVkDescriptorPool(VulkanMisc *vM, int count, VkDescriptorType type);
        static VkDescriptorSetLayout createVkDescriptorSetLayout(VulkanMisc *vM, int count, VkDescriptorType type, int descriptorCount);
        static VkDescriptorSet createVkDescriptorSet(VulkanMisc *vM, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool);
        static void destroyVkDescriptorSet(VulkanMisc *vM, VkDescriptorSet descriptorSets, VkDescriptorPool descriptorPool);
        static void destroyVkDescriptorPool(VulkanMisc *vM, VkDescriptorPool descriptorPool);
        static void destroyVkVkDescriptorSetLayout(VulkanMisc *vM, VkDescriptorSetLayout descriptorSetLayout);

    private:
		static int countDescriptor;
		static std::vector<Descriptor *> s_Descriptor;
        VulkanMisc *vulkanM;
        VkDescriptorSetLayout m_DescriptorSetLayout;
        VkDescriptorPool m_DescriptorPool;
        VkDescriptorSet m_DescriptorSets;
        VkDescriptorType m_descriptorType;
        int m_count = 0;
        int m_sizeOfResource = 0;
		int m_countDescriptor = 0;
    };
}

#endif //__ENGINE_DESCRIPTOR__