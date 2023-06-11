#ifndef __ENGINE_CONSOLE_DEBUG__
#define __ENGINE_CONSOLE_DEBUG__

#include <vector>
#include "Debug.hpp"
#include "imgui-cmake/Header/imgui.h"
#include "imgui-cmake/Header/imgui_impl_vulkan.h"
#include "imgui-cmake/Header/imgui_impl_glfw.h"
#include "VulkanMisc.hpp"

namespace Ge
{
	struct ConsoleDebug
	{
		ImGuiTextBuffer     Buf;
		ImGuiTextFilter     Filter;
		ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
		bool                AutoScroll;  // Keep scrolling if already at the bottom.
		std::vector<ImVec4> colorListe;
		ImColor corlorBase = ImColor(1.0f, 1.0f, 1.0f, 1.0f);

		ConsoleDebug()
		{
			AutoScroll = true;
			Clear();
		}

		void Clear()
		{
			Buf.clear();
			colorListe.clear();
			LineOffsets.clear();
			colorListe.push_back(ImColor(1.0f, 1.0f, 1.0f, 1.0f));
			LineOffsets.push_back(0);
		}

		void setBaseColor(ImColor ic)
		{
			corlorBase = ic;
		}

		void AddLog(const char* fmt, ...) IM_FMTARGS(2)
		{
			int old_size = Buf.size();
			va_list args;
			va_start(args, fmt);
			Buf.appendfv(fmt, args);
			va_end(args);
			for (int new_size = Buf.size(); old_size < new_size; old_size++)
			{
				if (Buf[old_size] == '\n')
				{
					LineOffsets.push_back(old_size + 1);
					colorListe.emplace_back(corlorBase);
				}
			}

		}

		void Draw(const char* title, VulkanMisc * vM, bool* p_open = NULL)
		{
			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoResize;
			window_flags |= ImGuiWindowFlags_NoCollapse;
			window_flags |= ImGuiWindowFlags_NoNav;
			ImGui::SetNextWindowBgAlpha(0.55f);
			if (!ImGui::Begin(title, p_open, window_flags))
			{
				ImGui::End();
				return;
			}
			ImGui::SetWindowPos(title, ImVec2(0, vM->str_VulkanSwapChainMisc->str_swapChainExtent.height - vM->str_VulkanSwapChainMisc->str_swapChainExtent.height / 3.55f));
			ImGui::SetWindowSize(title, ImVec2(vM->str_VulkanSwapChainMisc->str_swapChainExtent.width, vM->str_VulkanSwapChainMisc->str_swapChainExtent.height / 3.55f));
			// Options menu
			if (ImGui::BeginPopup("Options"))
			{
				ImGui::Checkbox("Auto-scroll", &AutoScroll);
				ImGui::EndPopup();
			}

			// Main window
			if (ImGui::Button("Options"))
				ImGui::OpenPopup("Options");
			ImGui::SameLine();
			bool clear = ImGui::Button("Clear");
			ImGui::SameLine();
			bool copy = ImGui::Button("Copy");
			ImGui::SameLine();
			Filter.Draw("Filter", -100.0f);

			ImGui::Separator();
			ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

			if (clear)
			{
				Clear();
			}
			if (copy)
			{
				ImGui::LogToClipboard();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			const char* buf = Buf.begin();
			const char* buf_end = Buf.end();
			if (Filter.IsActive())
			{
				for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
				{
					const char* line_start = buf + LineOffsets[line_no];
					const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
					if (Filter.PassFilter(line_start, line_end))
					{
						ImGui::PushStyleColor(ImGuiCol_Text, colorListe[(line_no + 1) % colorListe.size()]);
						ImGui::TextUnformatted(line_start, line_end);
						ImGui::PopStyleColor();
					}
				}
			}
			else
			{

				ImGuiListClipper clipper;
				clipper.Begin(LineOffsets.Size);
				while (clipper.Step())
				{
					for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
					{
						const char* line_start = buf + LineOffsets[line_no];
						const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
						ImGui::PushStyleColor(ImGuiCol_Text, colorListe[(line_no + 1) % colorListe.size()]);
						ImGui::TextUnformatted(line_start, line_end);
						ImGui::PopStyleColor();
					}
				}
				clipper.End();
			}
			ImGui::PopStyleVar();

			if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			ImGui::EndChild();
			ImGui::End();
		}
	};
}

#endif//__ENGINE_CONSOLE_DEBUG__