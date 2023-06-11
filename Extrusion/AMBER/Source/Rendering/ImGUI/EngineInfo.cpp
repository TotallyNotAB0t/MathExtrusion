#include "EngineInfo.hpp"

namespace Ge
{
	void EngineInfo::preRender(VulkanMisc* vM)
	{

	}

	void EngineInfo::render(VulkanMisc* vM)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

		window_flags |= ImGuiWindowFlags_NoMove;
		ImGui::SetNextWindowPos(ImVec2(vM->str_VulkanSwapChainMisc->str_swapChainExtent.width - 320.0f, 0), ImGuiCond_Always);

		ImGui::SetNextWindowBgAlpha(0.55f);
		if (ImGui::Begin("Example: Simple overlay", &m_open, window_flags))
		{
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Image par seconde : %.1f (FPS)", io.Framerate);
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Temps entre chaque image  : %.3f ms/frame", 1000.0f / io.Framerate);
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Nombre d'images : %d images", ImGui::GetFrameCount());
			ImGui::Text("Temps depuis demarage %.1f", ImGui::GetTime());
			ImGui::Text("Nombre de modeles : %d", vM->str_VulkanDescriptor->modelCount);
			ImGui::Text("Nombre de textures : %d", vM->str_VulkanDescriptor->textureCount);
			ImGui::Text("Nombre de materiaux : %d", vM->str_VulkanDescriptor->materialCount);
			ImGui::Text("Nombre de lumieres : %d", vM->str_VulkanDescriptor->lightCount);
			ImGui::Text("Carte Graphique (GPU): %s", vM->str_VulkanDeviceMisc->str_physicalDeviceProperties.deviceName);
			ImGui::Text("Resolution : %.0f x %.0f", io.DisplaySize.x, io.DisplaySize.y);
		}
		ImGui::End();
	}
}