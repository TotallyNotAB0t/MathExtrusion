#ifndef __ENGINE_MATERIAL_MANAGER__
#define __ENGINE_MATERIAL_MANAGER__

#include "Debug.hpp"
#include "VulkanMisc.hpp"
#include "Manager.hpp"
#include "Materials.hpp"
#include <map>

namespace Ge
{
    class MaterialManager : public Manager
    {
    public:
        bool initialize(VulkanMisc *vM);
        void release();
        Materials *createMaterial();
        void destroyMaterial(Materials *material);
		void initDescriptor(VulkanMisc * vM);
		void updateDescriptor();    
		static Materials * getDefaultMaterial();
    private:
		static Materials * defaultMaterial;
        std::vector<Materials *> m_materials;
        VulkanMisc *vulkanM;        
    };
}

#endif //__ENGINE_MATERIAL_MANAGER__