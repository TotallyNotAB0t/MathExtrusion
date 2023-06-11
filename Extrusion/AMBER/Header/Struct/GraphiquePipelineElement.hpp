#ifndef __ENGINE_GRAPHIQUE_PIPELINE_ELEMENT__
#define __ENGINE_GRAPHIQUE_PIPELINE_ELEMENT__

#include "vulkan/vulkan.h"

struct GraphiquePipelineElement
{
    VkPipeline m_graphicsPipeline;
    VkPipelineCache m_graphicsPipelineCache;
    VkPipelineLayout m_pipelineLayout;
};

#endif //__ENGINE_GRAPHIQUE_PIPELINE_ELEMENT__