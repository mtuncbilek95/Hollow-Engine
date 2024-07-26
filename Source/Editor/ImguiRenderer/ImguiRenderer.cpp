#include "ImguiRenderer.h"

#include <Engine/Window/WindowAPI.h>
#include <Engine/Graphics/API/GraphicsAPI.h>

#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

#include <Engine/Vulkan/Instance/VInstance.h>
#include <Engine/Vulkan/Device/VDevice.h>
#include <Engine/Vulkan/Queue/VQueue.h>
#include <Engine/Vulkan/Descriptor/VDescriptorPool.h>
#include <Engine/Vulkan/Command/VCmdPool.h>
#include <Engine/Vulkan/Command/VCmdBuffer.h>

#include <Engine/Graphics/Swapchain/Swapchain.h>
#include <Engine/Graphics/Sync/Fence.h>

namespace Hollow
{
	ImguiRenderer::ImguiRenderer()
	{
		auto instance = GraphicsAPI::GetAPI()->GetInstance();
		auto device = GraphicsAPI::GetAPI()->GetDevice();
		auto gQueue = GraphicsAPI::GetAPI()->GetGraphicsQueue();
		auto swapchain = GraphicsAPI::GetAPI()->GetSwapchain();

		DescriptorPoolDesc poolDesc = {};
		poolDesc.MaxSets = 1000;
		poolDesc.Flags = DescriptorPoolFlags::FreeDescriptorSet;
		poolDesc.PoolSizes = {
			{DescriptorType::CombinedImageSampler, 1000},
			{DescriptorType::UniformBuffer, 1000},
			{DescriptorType::StorageBuffer, 1000},
			{DescriptorType::StorageImage, 1000},
			{DescriptorType::StorageTexelBuffer, 1000},
			{DescriptorType::UniformTexelBuffer, 1000},
			{DescriptorType::InputAttachment, 1000},
			{DescriptorType::SampledImage, 1000},
			{DescriptorType::Sampler, 1000}
		};

		mDescriptorPool = device.lock()->CreateDescriptorPool(poolDesc);

		mContext = ImGui::CreateContext();
		ImGui::SetCurrentContext(mContext);

		ImGui_ImplVulkan_InitInfo initInfo = {};
		initInfo.Instance = instance.lock()->GetSharedPtrAs<VInstance>()->GetVkInstance();
		initInfo.PhysicalDevice = instance.lock()->GetSharedPtrAs<VInstance>()->GetVkAdapter();
		initInfo.Device = device.lock()->GetSharedPtrAs<VDevice>()->GetVkDevice();
		initInfo.QueueFamily = device.lock()->GetSharedPtrAs<VDevice>()->GetGraphicsQueueFamilyIndex();
		initInfo.Queue = gQueue.lock()->GetSharedPtrAs<VQueue>()->GetVkQueue();
		initInfo.DescriptorPool = mDescriptorPool->GetSharedPtrAs<VDescriptorPool>()->GetVkDescriptorPool();
		initInfo.MinImageCount = 2;
		initInfo.ImageCount = 2;
		initInfo.RenderPass = nullptr;
		initInfo.MinAllocationSize = 1024 * 1024;
		initInfo.UseDynamicRendering = true;

		const VkFormat format[] = { VK_FORMAT_R8G8B8A8_UNORM };
		VkPipelineRenderingCreateInfo renderingInfo = {};
		renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
		renderingInfo.colorAttachmentCount = 1;
		renderingInfo.depthAttachmentFormat = VK_FORMAT_UNDEFINED;
		renderingInfo.pColorAttachmentFormats = format;
		renderingInfo.viewMask = 0;
		renderingInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;

		initInfo.PipelineRenderingCreateInfo = renderingInfo;
		initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		ImGui_ImplVulkan_Init(&initInfo);

		GLFWwindow* window = WindowAPI::GetAPI()->GetDefaultWindow()->GetGLFWHandle();
		ImGui_ImplGlfw_InitForVulkan(window, true);

		CmdPoolDesc cmdPoolDesc = {};
		cmdPoolDesc.PoolType = CmdPoolType::Graphics;
		mCmdPool = device.lock()->CreateCommandPool(cmdPoolDesc);

		CmdBufferDesc cmdBufferDesc = {};
		cmdBufferDesc.pOwnerPool = mCmdPool;
		mCmdBuffer = device.lock()->CreateCommandBuffer(cmdBufferDesc);

		mGraphicsQueue = gQueue;

		mFence = device.lock()->CreateGraphicsFence(false);
	}

	ImguiRenderer::~ImguiRenderer()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext(mContext);
	}

	void ImguiRenderer::BeginFrame()
	{
		mCmdBuffer->BeginRecording();

		DynamicPassAttachmentDesc passColorAttachmentDesc = {};
		passColorAttachmentDesc.ImageBuffer = GraphicsAPI::GetAPI()->GetSwapchain().lock()->GetImageView(0);
		passColorAttachmentDesc.ImageLayout = TextureLayout::ColorAttachment;
		passColorAttachmentDesc.LoadOperation = AttachmentLoadOperation::Clear;
		passColorAttachmentDesc.StoreOperation = AttachmentStoreOperation::Store;
		passColorAttachmentDesc.ClearColor = { 0.1f, 0.2f, 0.3f, 1.0f };

		DynamicPassDesc passDesc = {};
		passDesc.ColorAttachments = { passColorAttachmentDesc };
		passDesc.layerCount = 1;
		passDesc.Extent = { GraphicsAPI::GetAPI()->GetSwapchain().lock()->GetImageSize().x, GraphicsAPI::GetAPI()->GetSwapchain().lock()->GetImageSize().y };
		passDesc.Offset = { 0, 0 };
		passDesc.viewMask = 0;

		mCmdBuffer->BeginRendering(passDesc);

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();
	}

	void ImguiRenderer::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), mCmdBuffer->GetSharedPtrAs<VCmdBuffer>()->GetVkCmdBuffer());

		mCmdBuffer->EndRendering();
		mCmdBuffer->EndRecording();

		PipelineStageFlags flags[] = { PipelineStageFlags::ColorAttachmentOutput };
		GraphicsAPI::GetAPI()->GetDevice().lock()->SubmitQueue(mGraphicsQueue, mCmdBuffer, 1, nullptr, 0, nullptr, 0, mFence, flags);

		GraphicsAPI::GetAPI()->GetDevice().lock()->WaitFence(mFence);
		GraphicsAPI::GetAPI()->GetDevice().lock()->ResetFence(mFence);
	}
}
