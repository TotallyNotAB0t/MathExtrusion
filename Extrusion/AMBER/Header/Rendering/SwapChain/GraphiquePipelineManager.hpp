#ifndef __ENGINE_GRAPHIQUE_PIPELINE_MANAGER__
#define __ENGINE_GRAPHIQUE_PIPELINE_MANAGER__

#include "VulkanMisc.hpp"
#include "Debug.hpp"
#include <vector>
#include "GraphiquePipeline.hpp"
#include "ShaderPair.hpp"
#include <map>

namespace Ge
{
    class GraphiquePipelineManager
    {
    public:
        bool initialize(VulkanMisc *vM);
        void release();
		GraphiquePipeline * createPipeline(const std::string & frag,const std::string & vert,bool back = false, bool multiS = true);
        void destroyPipeline(GraphiquePipeline * pipeline);
		static std::vector<GraphiquePipeline *> GetPipelines();
    private:
        VulkanMisc *vulkanM;        
        std::vector<ShaderPair *> m_fileNameShaders;        
        static std::vector<GraphiquePipeline *> m_graphiquePipeline;

    };
}

#endif //__ENGINE_GRAPHIQUE_PIPELINE_MANAGER__