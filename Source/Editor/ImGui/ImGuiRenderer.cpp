#include "ImGuiRenderer.h"

#include <Runtime/Vulkan/Device/VulkanDevice.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorPool.h>
#include <Runtime/Vulkan/Queue/VulkanQueue.h>
#include <Runtime/Vulkan/Core/VulkanCoreUtils.h>
#include <Runtime/Vulkan/Command/VulkanCommandBuffer.h>

#include <Runtime/Window/WindowManager.h>

#define IMGUI_IMPL_VULKAN_HAS_DYNAMIC_RENDERING
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_vulkan.h>

#define IMGUI_BUFFER_COUNT 2

namespace Hollow
{
	ImGuiRenderer::ImGuiRenderer()
	{
		mDevice = GraphicsManager::GetInstanceAPI().GetDefaultDevice();
	}

	void ImGuiRenderer::CreateImguiResources()
	{
	}

	void ImGuiRenderer::UpdateRender()
	{
	}

	void ImGuiRenderer::OnShutdown()
	{
	}
}
