#include "Hud.hpp"
#include "ColorResources.hpp"
#include "PhysicalDevices.hpp"
#include "SwapChain.hpp"

namespace Ge
{
	Hud::Hud()
	{		
		//m_imguiBlock.push_back(&m_engineInfo);
		//m_imguiBlock.push_back(&m_console);
		//m_imguiBlock.push_back(&m_hiearchy);
	}

	bool Hud::initialize(VulkanMisc* vM)
	{
		IMGUI_CHECKVERSION();
		vulkanM = vM;
		//vulkanM = vM;
		ImGui::CreateContext();
		ImPlot::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		//ImGui::StyleColorsClassic();
		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		ImVec4* colors = style.Colors;
		colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.11f, 0.14f, 0.92f);
		colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.0f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.43f, 0.43f, 0.43f, 0.1f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.49f, 0.27f, 0.27f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.21f, 0.22f, 0.69f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.34f, 0.07f, 0.07f, 0.83f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.43f, 0.12f, 0.12f, 0.87f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.60f, 0.20f, 0.20f, 0.20f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.03f, 0.03f, 0.80f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01f, 0.00f, 0.00f, 0.30f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.60f, 0.20f, 0.20f, 0.30f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.60f, 0.20f, 0.20f, 0.40f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.60f, 0.19f, 0.21f, 0.60f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
		colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.60f, 0.19f, 0.21f, 0.60f);
		colors[ImGuiCol_Button] = ImVec4(0.25f, 0.05f, 0.05f, 0.62f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.10f, 0.10f, 0.79f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.60f, 0.10f, 0.10f, 0.45f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.60f, 0.15f, 0.15f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.57f, 0.23f, 0.23f, 0.80f);
		colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.50f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.16f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.82f, 0.78f, 0.60f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.82f, 0.78f, 0.90f);
		colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);

		VkAttachmentDescription attachment = {};
		attachment.format = vM->str_VulkanSwapChainMisc->str_swapChainImageFormat;
		attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference color_attachment = {};
		color_attachment.attachment = 0;
		color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &color_attachment;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;  // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.attachmentCount = 1;
		info.pAttachments = &attachment;
		info.subpassCount = 1;
		info.pSubpasses = &subpass;
		info.dependencyCount = 1;
		info.pDependencies = &dependency;
		if (vkCreateRenderPass(vM->str_VulkanDeviceMisc->str_device, &info, nullptr, &m_imGuiRenderPass) != VK_SUCCESS)
		{
			Debug::Error("Echec de l'intialisation de la render pass pour imGUI");
			return false;
		}

		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(11);
		poolInfo.pPoolSizes = pool_sizes;
		poolInfo.maxSets = static_cast<uint32_t>(vM->str_VulkanSwapChainMisc->str_swapChainImages.size());

		if (vkCreateDescriptorPool(vM->str_VulkanDeviceMisc->str_device, &poolInfo, nullptr, &m_imGuiDescriptorPool) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation d'un descriptor pool pour ImGUI");
			return false;
		}

		ImGui_ImplGlfw_InitForVulkan(vM->str_VulkanDeviceMisc->str_window, true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = vM->str_VulkanDeviceMisc->str_instance;
		init_info.PhysicalDevice = vM->str_VulkanDeviceMisc->str_physicalDevice;
		init_info.Device = vM->str_VulkanDeviceMisc->str_device;
		init_info.QueueFamily = PhysicalDevices::getCountQueueFamily(vM->str_VulkanDeviceMisc->str_physicalDevice);
		init_info.Queue = vM->str_VulkanDeviceMisc->str_graphicsQueue;
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = m_imGuiDescriptorPool;
		init_info.Allocator = nullptr;
		init_info.MinImageCount = vM->str_VulkanSwapChainMisc->str_imageCount - 1;
		init_info.ImageCount = vM->str_VulkanSwapChainMisc->str_imageCount;
		init_info.CheckVkResultFn = check_vk_result;
		ImGui_ImplVulkan_Init(&init_info, m_imGuiRenderPass);

		m_clearValues[0].color = { 0.682f, 0.811f, 0.917f, 1.0f };
		m_clearValues[1].depthStencil = { 1.0f, 0 };

		if (!createCommandPool(&m_imGuiCommandPools, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, vM->str_VulkanDeviceMisc))
		{
			return false;
		}

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_imGuiCommandPools;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(vM->str_VulkanDeviceMisc->str_device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);
		ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(vM->str_VulkanDeviceMisc->str_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(vM->str_VulkanDeviceMisc->str_graphicsQueue);

		m_imGuiCommandBuffers.resize(vM->str_VulkanSwapChainMisc->str_swapChainImageViews.size());
		if (!createCommandBuffers(m_imGuiCommandBuffers.data(), static_cast<uint32_t>(m_imGuiCommandBuffers.size()), m_imGuiCommandPools, vM->str_VulkanDeviceMisc))
		{
			return false;
		}

		m_imGuiFramebuffer.resize(vM->str_VulkanSwapChainMisc->str_swapChainImages.size());
		VkFramebufferCreateInfo fbinfo = {};
		fbinfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbinfo.renderPass = m_imGuiRenderPass;
		fbinfo.attachmentCount = 1;
		fbinfo.pAttachments = nullptr;
		fbinfo.width = vM->str_VulkanSwapChainMisc->str_swapChainExtent.width;
		fbinfo.height = vM->str_VulkanSwapChainMisc->str_swapChainExtent.height;
		fbinfo.layers = 1;
		for (uint32_t i = 0; i < vM->str_VulkanSwapChainMisc->str_swapChainImages.size(); i++)
		{
			fbinfo.pAttachments = &vM->str_VulkanSwapChainMisc->str_swapChainImageViews[i];
			if (vkCreateFramebuffer(vM->str_VulkanDeviceMisc->str_device, &fbinfo, nullptr, &m_imGuiFramebuffer[i]) != VK_SUCCESS)
			{
				Debug::Error("Echec de la creation d'un frame buffer pour imGUi");
				return false;
			}
		}
		return true;
	}

	ImFont * Hud::createFont(const char* filename, float size_pixels, const ImFontConfig* font_cfg, const ImWchar* glyph_ranges)
	{		
		ImFont * fontBuild = nullptr;
		//vkDeviceWaitIdle(vulkanM->str_VulkanDeviceMisc->str_device);		
		ImGuiIO& io = ImGui::GetIO();
		fontBuild = io.Fonts->AddFontFromFileTTF(filename, size_pixels, font_cfg, glyph_ranges);		
		io.Fonts->Build();		
		VkCommandBuffer cmd = BufferManager::beginSingleTimeCommands(vulkanM);
		ImGui_ImplVulkan_DestroyFontUploadObjects();
		ImGui_ImplVulkan_DestroyFontsTexture();
		ImGui_ImplVulkan_CreateFontsTexture(cmd);
		BufferManager::endSingleTimeCommands(cmd, vulkanM);
		ImGui_ImplVulkan_DestroyFontUploadObjects();
		return fontBuild;
	}

	void Hud::check_vk_result(VkResult err)
	{
		if (err != VK_SUCCESS)
		{
			Debug::Error("check_vk_result IMGUI : %d", nullptr, err);
		}
	}

	bool Hud::getHudActive()
	{
		return m_hudActive;
	}

	bool Hud::getGlobalHud()
	{
		return m_globalUI;
	}

	void Hud::setHudActive(bool state)
	{
		m_hudActive = state;
	}

	void Hud::setGlobalHud(bool state)
	{
		m_globalUI = state;
	}

	bool Hud::createCommandPool(VkCommandPool* commandPool, VkCommandPoolCreateFlags flags, VulkanDeviceMisc* vM)
	{
		VkCommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.queueFamilyIndex = PhysicalDevices::getGraphiqueIndexFamily(vM->str_physicalDevice);
		commandPoolCreateInfo.flags = flags;

		if (vkCreateCommandPool(vM->str_device, &commandPoolCreateInfo, nullptr, commandPool) != VK_SUCCESS)
		{
			Debug::Error("Echec de la creation d'un commandPool pour imGUI");
			return false;
		}
		for (int i = 0; i < m_imguiBlockExtern.size(); i++)
		{
			m_imguiBlockExtern[i]->preRender(vulkanM);
		}
		return true;
	}

	bool Hud::createCommandBuffers(VkCommandBuffer* commandBuffer, uint32_t commandBufferCount, VkCommandPool& commandPool, VulkanDeviceMisc* vM)
	{
		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocateInfo.commandPool = commandPool;
		commandBufferAllocateInfo.commandBufferCount = commandBufferCount;
		vkAllocateCommandBuffers(vM->str_device, &commandBufferAllocateInfo, commandBuffer);
		return true;
	}

	void Hud::release()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->Clear();
		for (auto framebuffer : m_imGuiFramebuffer)
		{
			vkDestroyFramebuffer(vulkanM->str_VulkanDeviceMisc->str_device, framebuffer, nullptr);
		}
		vkDestroyRenderPass(vulkanM->str_VulkanDeviceMisc->str_device, m_imGuiRenderPass, nullptr);
		vkFreeCommandBuffers(vulkanM->str_VulkanDeviceMisc->str_device, m_imGuiCommandPools, static_cast<uint32_t>(m_imGuiCommandBuffers.size()), m_imGuiCommandBuffers.data());
		vkDestroyCommandPool(vulkanM->str_VulkanDeviceMisc->str_device, m_imGuiCommandPools, nullptr);
		vkDestroyDescriptorPool(vulkanM->str_VulkanDeviceMisc->str_device, m_imGuiDescriptorPool, nullptr);

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		ImPlot::DestroyContext();
		Debug::RELEASESUCCESS("ImGUI");
	}

	void Hud::recreateSwapChain()
	{
		for (auto framebuffer : m_imGuiFramebuffer)
		{
			vkDestroyFramebuffer(vulkanM->str_VulkanDeviceMisc->str_device, framebuffer, nullptr);
		}

		m_imGuiFramebuffer.resize(vulkanM->str_VulkanSwapChainMisc->str_swapChainImages.size());
		VkFramebufferCreateInfo fbinfo = {};
		fbinfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbinfo.renderPass = m_imGuiRenderPass;
		fbinfo.attachmentCount = 1;
		fbinfo.pAttachments = nullptr;
		fbinfo.width = vulkanM->str_VulkanSwapChainMisc->str_swapChainExtent.width;
		fbinfo.height = vulkanM->str_VulkanSwapChainMisc->str_swapChainExtent.height;
		fbinfo.layers = 1;
		for (uint32_t i = 0; i < vulkanM->str_VulkanSwapChainMisc->str_swapChainImages.size(); i++)
		{
			fbinfo.pAttachments = &vulkanM->str_VulkanSwapChainMisc->str_swapChainImageViews[i];
			if (vkCreateFramebuffer(vulkanM->str_VulkanDeviceMisc->str_device, &fbinfo, nullptr, &m_imGuiFramebuffer[i]) != VK_SUCCESS)
			{
				Debug::Error("Echec de la creation d'un frame buffer pour imGUi");
			}
		}
	}

	void Hud::addBlockUI(ImguiBlock * ib)
	{
		m_imguiBlockExtern.push_back(ib);
		ib->preRender(vulkanM);
	}

	void Hud::removeBlockUI(ImguiBlock * ib)
	{
		m_imguiBlockExtern.erase(std::remove(m_imguiBlockExtern.begin(), m_imguiBlockExtern.end(), ib), m_imguiBlockExtern.end());
		//ImGui_ImplVulkan_DestroyFontsTexture(); //TODO a modifier
	}

	void Hud::render(uint32_t currentframe)
	{
		//vkResetCommandPool(vulkanM->str_VulkanDeviceMisc->str_device, m_imGuiCommandPools, 0);
		VkCommandBufferBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		vkBeginCommandBuffer(m_imGuiCommandBuffers[currentframe], &info);
		Hud::imGuiRender();
		ImGui::Render();


		VkRenderPassBeginInfo binfo = {};
		binfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		binfo.renderPass = m_imGuiRenderPass;
		binfo.framebuffer = m_imGuiFramebuffer[currentframe];
		binfo.renderArea.extent.width = vulkanM->str_VulkanSwapChainMisc->str_swapChainExtent.width;
		binfo.renderArea.extent.height = vulkanM->str_VulkanSwapChainMisc->str_swapChainExtent.height;
		binfo.clearValueCount = static_cast<uint32_t>(m_clearValues.size());
		binfo.pClearValues = m_clearValues.data();
		vkCmdBeginRenderPass(m_imGuiCommandBuffers[currentframe], &binfo, VK_SUBPASS_CONTENTS_INLINE);

		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_imGuiCommandBuffers[currentframe]);

		vkCmdEndRenderPass(m_imGuiCommandBuffers[currentframe]);

		vkEndCommandBuffer(m_imGuiCommandBuffers[currentframe]);
	}
	void Hud::imGuiRender()
	{
		bool openInfo = true;

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		if (m_globalUI)
		{
			for (int i = 0; i < m_imguiBlockExtern.size(); i++)
			{
				m_imguiBlockExtern[i]->render(vulkanM);
			}
			if (m_hudActive)
			{
				for (int i = 0; i < m_imguiBlock.size(); i++)
				{					
					m_imguiBlock[i]->render(vulkanM);
				}
			}
		}
	}
}


