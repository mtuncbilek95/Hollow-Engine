#include "ImGuiRenderer.h"

#include <Engine/Window/WindowAPI.h>
#include <Engine/Graphics/API/GraphicsAPI.h>
#include <Engine/Platform/API/PlatformAPI.h>
#include <Engine/Platform/PlatformMonitor.h>

#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

#include <Engine/Vulkan/Instance/VInstance.h>
#include <Engine/Vulkan/Device/VDevice.h>
#include <Engine/Vulkan/Queue/VQueue.h>
#include <Engine/Vulkan/Descriptor/VDescriptorPool.h>

#include <Editor/Utils/ImGuiUtils.h>

const VkFormat format[1] = { VK_FORMAT_R8G8B8A8_UNORM };

namespace Hollow
{
	ImGuiRenderer::ImGuiRenderer()
	{
		auto device = GraphicsAPI::GetAPI()->GetDevice();

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

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
		io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;

		// Win32 should trigger the scale factor.
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

		ImGuiUtils::LoadStyleLayout();

		// FramebufferSize
		auto window = WindowAPI::GetAPI()->GetDefaultWindow();

		String path = PlatformAPI::GetAPI()->GetEngineSourcePath() + "Resources/Fonts/Poppins/Regular.ttf";
		io.Fonts->AddFontFromFileTTF(path.c_str(), 18.f);

		InitVk();
	}

	ImGuiRenderer::~ImGuiRenderer()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext(mContext);
	}

	void ImGuiRenderer::InitVk()
	{
		auto instance = GraphicsAPI::GetAPI()->GetInstance();
		auto device = GraphicsAPI::GetAPI()->GetDevice();
		auto gQueue = GraphicsAPI::GetAPI()->GetGraphicsQueue();

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

		VkPipelineRenderingCreateInfo renderingInfo = {};
		renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
		renderingInfo.colorAttachmentCount = 1;
		renderingInfo.depthAttachmentFormat = VK_FORMAT_UNDEFINED;
		renderingInfo.pColorAttachmentFormats = format;
		renderingInfo.viewMask = 0;
		renderingInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;

		initInfo.PipelineRenderingCreateInfo = renderingInfo;
		initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		GLFWwindow* window = WindowAPI::GetAPI()->GetDefaultWindow()->GetGLFWHandle();
		ImGui_ImplGlfw_InitForVulkan(window, true);

		ImGui_ImplVulkan_Init(&initInfo);

		CmdPoolDesc poolDesc = {};
		poolDesc.PoolType = CmdPoolType::Graphics;
		mCmdPool = device.lock()->CreateCommandPool(poolDesc);

		CmdBufferDesc bufferDesc = {};
		bufferDesc.pOwnerPool = mCmdPool;
		mCmdBuffer = device.lock()->CreateCommandBuffer(bufferDesc);
	}
}
