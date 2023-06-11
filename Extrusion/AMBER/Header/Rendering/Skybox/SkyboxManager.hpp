#ifndef __ENGINE_SKYBOX_MANAGER__
#define __ENGINE_SKYBOX_MANAGER__

#include "Manager.hpp"
#include "Debug.hpp"
#include "Skybox.hpp"
#include "VulkanMisc.hpp"
#include "ModelManager.hpp"
#include "GraphiquePipelineManager.hpp"

namespace Ge
{
	class SkyboxManager : public Manager
	{
	public:
		bool initialize(VulkanMisc * vM, ModelManager* mM, GraphiquePipelineManager * gPM);
		void release();
		void initDescriptor(VulkanMisc * vM);
		Skybox * loadSkybox(TextureCubeMap * tCM);
		Skybox* createSkybox(TextureCubeMap* tCM);
		void changeSkybox(Skybox * sky);
		void updateDescriptor();		
		static Skybox * GetCurrentSkybox();
	private:
		VulkanMisc * vulkanM;		
		ShapeBuffer * cubeMapBaseModel;
		GraphiquePipeline * m_skyboxPipeline;
		static Skybox * currentSkybox;
		std::vector<Skybox*> m_skybox;
	};
}

#endif//__ENGINE_SKYBOX_MANAGER__