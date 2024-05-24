#include "ImGuiRenderer.h"

#include <Runtime/Vulkan/Device/VulkanDevice.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorPool.h>
#include <Runtime/Vulkan/Queue/VulkanQueue.h>
#include <Runtime/Vulkan/Core/VulkanCoreUtils.h>
#include <Runtime/Vulkan/Command/VulkanCommandBuffer.h>

#include <Runtime/Window/WindowManager.h>
#include <Runtime/Win32/Win32Window.h>

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
		IMGUI_CHECKVERSION();
		mContext = ImGui::CreateContext();
		ImGui::StyleColorsDark();

		auto win32Window = std::dynamic_pointer_cast<Win32Window>(WindowManager::GetInstanceAPI().GetDefaultWindow());
		CORE_ASSERT(ImGui_ImplWin32_Init(win32Window->GetHandle()), "ImGuiRenderer", "Failed to initialize ImGui Win32 backend!");

		auto vkDevice = std::dynamic_pointer_cast<VulkanDevice>(mDevice);

		DescriptorPoolDesc poolDesc = {};
		poolDesc.MaxSets = 1000;
		poolDesc.PoolSizes = {
			{DescriptorType::Sampler, 1000},
			{DescriptorType::CombinedImageSampler, 1000},
			{DescriptorType::SampledImage, 1000},
			{DescriptorType::StorageImage, 1000},
			{DescriptorType::UniformTexelBuffer, 1000},
			{DescriptorType::StorageTexelBuffer, 1000},
			{DescriptorType::UniformBuffer, 1000},
			{DescriptorType::StorageBuffer, 1000},
			{DescriptorType::UniformBufferDynamic, 1000},
			{DescriptorType::StorageBufferDynamic, 1000},
			{DescriptorType::InputAttachment, 1000}
		};

		mDescriptorPool = mDevice->CreateDescriptorPool(poolDesc);

		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = vkDevice->GetVkInstance();
		init_info.PhysicalDevice = vkDevice->GetVkPhysicalDevice();
		init_info.Device = vkDevice->GetVkDevice();
		init_info.QueueFamily = vkDevice->GetQueueFamilyIndex(GraphicsQueueType::Graphics);
		init_info.Queue = std::static_pointer_cast<VulkanQueue>(GraphicsManager::GetInstanceAPI().GetDefaultPresentQueue())->GetVkQueue();
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = std::static_pointer_cast<VulkanDescriptorPool>(mDescriptorPool)->GetVkDescriptorPool();
		init_info.RenderPass = VK_NULL_HANDLE;
		init_info.UseDynamicRendering = true;
		init_info.PipelineRenderingCreateInfo = VulkanCoreUtils::CreatePipelineRenderingInfo();
		init_info.Allocator = nullptr;
		init_info.MinAllocationSize = 1024 * 1024;
		init_info.MinImageCount = IMGUI_BUFFER_COUNT;
		init_info.ImageCount = IMGUI_BUFFER_COUNT;
		init_info.CheckVkResultFn = nullptr;
		
		CORE_ASSERT(ImGui_ImplVulkan_Init(&init_info), "ImGuiRenderer", "Failed to initialize ImGui Vulkan backend!");

		CommandPoolDesc cmdPoolDesc = {};
		cmdPoolDesc.PoolType = CommandPoolType::Graphics;
		mCommandPool = mDevice->CreateCommandPool(cmdPoolDesc);

		CommandBufferDesc cmdBufferDesc = {};
		cmdBufferDesc.pOwnerPool = mCommandPool;
		mCommandBuffer = mDevice->CreateCommandBuffer(cmdBufferDesc);
	}

	void ImGuiRenderer::UpdateRender()
	{
	}

	void ImGuiRenderer::OnShutdown()
	{
	}
}
