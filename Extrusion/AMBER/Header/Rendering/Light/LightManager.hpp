#ifndef __ENGINE_LIGHT_MANAGER__
#define __ENGINE_LIGHT_MANAGER__

#include "VulkanMisc.hpp"
#include "Debug.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"
#include "DirectionalLight.hpp"
#include "Manager.hpp"

namespace Ge
{
    class LightManager : public Manager
    {
    public:
        bool initialize(VulkanMisc *vM);
        void release();
        SpotLight *createSpotLight(glm::vec3 position, glm::vec3 color, glm::vec3 euler, float angle, std::string name = "SpotLight");
        DirectionalLight *createDirectionalLight(glm::vec3 euler, glm::vec3 color, std::string name = "DirectionalLight");
        PointLight *createPointLight(glm::vec3 position, glm::vec3 color, std::string name = "PointLight");
        void destroyLight(Lights *light);
        void updateDescriptor();
        void majIndex();
		void initDescriptor(VulkanMisc * vM);
	private:
		friend class RenderingEngine;
		void destroyElement();
    private:
        VulkanMisc *vulkanM;
        std::vector<Lights *> m_lights;
		std::vector<Lights *> m_destroy_lights;
        VmaBuffer m_vmaUniformBuffers;
    };
}

#endif //__ENGINE_LIGHT_MANAGER__