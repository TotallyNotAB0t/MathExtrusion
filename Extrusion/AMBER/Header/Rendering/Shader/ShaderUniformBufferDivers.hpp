#ifndef __ENGINE_SHADER_UNIFORM_BUFFER_DIVERS__
#define __ENGINE_SHADER_UNIFORM_BUFFER_DIVERS__

#include "Debug.hpp"
#include "VulkanMisc.hpp"
#include "UniformBufferDiver.hpp"
#include "BufferManager.hpp"
#include "Manager.hpp"
#include "SettingManager.hpp"

namespace Ge
{
	class ShaderUniformBufferDivers : public Manager
	{
	public:
		bool initialize(VulkanMisc *vM, SettingManager * sM);
		void release();
		void updateUniformBufferDiver();
		void initDescriptor(VulkanMisc * vM);
		void updateDescriptor();
	private:
		SettingManager * settingM;
		VulkanMisc * vulkanM;
		UniformBufferDiver m_ubd;
		VmaBuffer m_vmaUniformBuffer;
	};
}

#endif //__ENGINE_SHADER_UNIFORM_BUFFER_DIVERS__