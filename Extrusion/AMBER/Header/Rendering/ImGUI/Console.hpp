#ifndef __ENGINE_CONSOLE__
#define __ENGINE_CONSOLE__

#include "ConsoleDebug.hpp"
#include "ImguiBlock.hpp"

namespace Ge
{
	class Console : public ImguiBlock
	{
	public:
		Console();
		void preRender(VulkanMisc* vM);
		void render(VulkanMisc* vM);
		static ConsoleDebug * GetConsole();
		static bool IsInit();
	private:
		static Console * s_pInstance;
		ConsoleDebug m_consoleDebug;
		bool m_open = true;
	};
}

#endif//__ENGINE_CONSOLE__