#include "ImGuiEndModule.h"

#include <Engine/Graphics/API/GraphicsAPI.h>
#include <Engine/Graphics/Device/GraphicsDevice.h>
#include <Engine/Graphics/Swapchain/Swapchain.h>
#include <Engine/Graphics/Queue/GraphicsQueue.h>
#include <Engine/Graphics/Command/CmdBuffer.h>
#include <Engine/Graphics/Command/CmdPool.h>

#include <Engine/Vulkan/Command/VCmdBuffer.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include <Editor/API/ImGuiAPI.h>

namespace Hollow
{
	ImGuiEndModule::ImGuiEndModule()
	{
		SetModuleName("ImGuiEndModule");
	}

	void ImGuiEndModule::Start() noexcept
	{
	}

	void ImGuiEndModule::Update() noexcept
	{
		ImGui::ShowDemoWindow(); // Test

		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), mCmdBuffer.lock()->GetSharedPtrAs<VCmdBuffer>()->GetVkCmdBuffer());
	}

	void ImGuiEndModule::Stop() noexcept
	{
	}

	bool ImGuiEndModule::OnInitialize() noexcept
	{
		mDevice = GraphicsAPI::GetAPI()->GetDevice();

		if (mDevice.expired())
			return false;

		mSwapchain = GraphicsAPI::GetAPI()->GetSwapchain();

		if (mSwapchain.expired())
			return false;

		mGraphicsQueue = GraphicsAPI::GetAPI()->GetGraphicsQueue();
		if (mGraphicsQueue.expired())
			return false;

		mFence = mDevice.lock()->CreateGraphicsFence(false);
		if (!mFence)
			return false;

		CmdPoolDesc poolDesc = {};
		poolDesc.PoolType = CmdPoolType::Graphics;


		mCmdBuffer = ImGuiAPI::GetAPI()->GetEditorRenderer().lock()->GetCmdBuffer();

		return true;
	}

	void ImGuiEndModule::OnPreUpdate() noexcept
	{
	}

	void ImGuiEndModule::OnPostUpdate() noexcept
	{
		mCmdBuffer.lock()->EndRendering();

		u32 index = mSwapchain.lock()->GetImageIndex();

		TextureImageBarrier postRenderBarrier = {};
		postRenderBarrier.ArrayIndex = 0;
		postRenderBarrier.MipIndex = 0;
		postRenderBarrier.SourceAccessMask = GraphicsMemoryAccessFlags::ColorAttachmentWrite;
		postRenderBarrier.SourceQueue = GraphicsQueueType::Graphics;
		postRenderBarrier.SourceStageFlags = PipelineStageFlags::ColorAttachmentOutput;
		postRenderBarrier.OldLayout = TextureLayout::ColorAttachment;

		postRenderBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::MemoryRead;
		postRenderBarrier.DestinationQueue = GraphicsQueueType::Graphics;
		postRenderBarrier.DestinationStageFlags = PipelineStageFlags::BottomOfPipe;
		postRenderBarrier.NewLayout = TextureLayout::Present;

		postRenderBarrier.AspectMask = TextureAspectFlags::ColorAspect;

		mCmdBuffer.lock()->SetTextureBarrier(mSwapchain.lock()->GetImage(index), postRenderBarrier);

		mCmdBuffer.lock()->EndRecording();

		auto flag = PipelineStageFlags::ColorAttachmentOutput;
		mDevice.lock()->SubmitQueue(mGraphicsQueue, mCmdBuffer, 1, nullptr, 0, nullptr, 0, mFence, &flag);
		mDevice.lock()->WaitFence(mFence);
		mDevice.lock()->ResetFence(mFence);

		mSwapchain.lock()->Present(nullptr);
	}
}
