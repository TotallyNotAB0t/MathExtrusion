#include "Console.hpp"

namespace Ge
{
	Console * Console::s_pInstance = nullptr;
	void Console::render(VulkanMisc* vM)
	{
		m_consoleDebug.Draw("Console", vM, &m_open);
	}

	void Console::preRender(VulkanMisc* vM)
	{

	}

	Console::Console()
	{
		Console::s_pInstance = this;
	}

	bool Console::IsInit()
	{
		return Console::s_pInstance != nullptr;
	}

	ConsoleDebug * Console::GetConsole()
	{
		return &(Console::s_pInstance->m_consoleDebug);
	}
}