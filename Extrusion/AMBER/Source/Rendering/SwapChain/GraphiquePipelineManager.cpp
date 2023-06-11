#include "GraphiquePipelineManager.hpp"

namespace Ge
{
	std::vector<GraphiquePipeline *> GraphiquePipelineManager::m_graphiquePipeline;
	bool GraphiquePipelineManager::initialize(VulkanMisc *vM)
	{
		vulkanM = vM;
		for (int i = 0; i < m_fileNameShaders.size(); i++)
		{
			createPipeline(m_fileNameShaders[i]->Frag, m_fileNameShaders[i]->Vert, m_fileNameShaders[i]->back);
		}
		if (m_fileNameShaders.size() == 0)
		{
			createPipeline("../Shader/frag.spv", "../Shader/vert.spv");			
		}
		return true;
	}

	void GraphiquePipelineManager::release()
	{
		m_fileNameShaders.clear();
		for (int i = 0; i < m_graphiquePipeline.size();i++)
		{			
			m_fileNameShaders.push_back(m_graphiquePipeline[i]->getShaderPair());
			delete (m_graphiquePipeline[i]);
		}
		m_graphiquePipeline.clear();
	}

	GraphiquePipeline * GraphiquePipelineManager::createPipeline(const std::string &frag, const std::string &vert,bool back,bool multiS)
	{
		ShaderPair * sp = new ShaderPair(frag, vert, back,multiS);
		GraphiquePipeline * gp = new GraphiquePipeline(vulkanM, sp);
		m_graphiquePipeline.push_back(gp);				
		return gp;
	}

	std::vector<GraphiquePipeline *> GraphiquePipelineManager::GetPipelines()
	{
		return m_graphiquePipeline;
	}

	void GraphiquePipelineManager::destroyPipeline(GraphiquePipeline * pipeline)
	{
		m_graphiquePipeline.erase(std::remove(m_graphiquePipeline.begin(), m_graphiquePipeline.end(), pipeline), m_graphiquePipeline.end());
		delete (pipeline);
	}
}