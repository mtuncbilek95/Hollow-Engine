#include "ImGuiBeginModule.h"

#include <Engine/Graphics/API/GraphicsAPI.h>
#include <Engine/Graphics/Device/GraphicsDevice.h>
#include <Engine/Graphics/Swapchain/Swapchain.h>
#include <Engine/Graphics/Queue/GraphicsQueue.h>
#include <Engine/Graphics/Command/CmdBuffer.h>
#include <Engine/Graphics/Command/CmdPool.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include <Editor/API/ImGuiAPI.h>

namespace Hollow
{
	ImGuiBeginModule::ImGuiBeginModule()
	{
		SetModuleName("ImGuiBeginModule");
	}

	void ImGuiBeginModule::Start() noexcept
	{
		mCmdBuffer.lock()->BeginRecording();

		for (u8 i = 0; i < mSwapchain.lock()->GetBufferCount(); ++i)
		{
			WeakPtr<TextureImage> image = mSwapchain.lock()->GetImage(i);
			if (image.expired())
				continue;

			TextureImageBarrier barrier = {};
			barrier.ArrayIndex = 0;
			barrier.MipIndex = 0;
			barrier.SourceAccessMask = GraphicsMemoryAccessFlags::Unknown;
			barrier.DestinationAccessMask = GraphicsMemoryAccessFlags::ColorAttachmentRead;
			barrier.SourceQueue = GraphicsQueueType::Graphics;
			barrier.DestinationQueue = GraphicsQueueType::Graphics;
			barrier.OldLayout = TextureLayout::Undefined;
			barrier.NewLayout = TextureLayout::Present;
			barrier.SourceStageFlags = PipelineStageFlags::TopOfPipe;
			barrier.DestinationStageFlags = PipelineStageFlags::ColorAttachmentOutput;

			barrier.AspectMask = TextureAspectFlags::ColorAspect;

			mCmdBuffer.lock()->SetTextureBarrier(image, barrier);
		}

		mCmdBuffer.lock()->EndRecording();

		PipelineStageFlags waitStage = PipelineStageFlags::ColorAttachmentOutput;
		mDevice.lock()->SubmitQueue(mGraphicsQueue.lock(), mCmdBuffer, 1, nullptr, 0, nullptr, 0, mFence, &waitStage);

		mDevice.lock()->WaitFence(mFence);
		mDevice.lock()->ResetFence(mFence);
	}

	void ImGuiBeginModule::Update() noexcept
	{
		u32 index = mSwapchain.lock()->AcquireNextImage(mFence, nullptr);
		mDevice.lock()->WaitFence(mFence);
		mDevice.lock()->ResetFence(mFence);

		mCmdBuffer.lock()->BeginRecording();

		WeakPtr<TextureImage> image = mSwapchain.lock()->GetImage(index);
		if (image.expired())
			return;

		TextureImageBarrier barrier = {};
		barrier.ArrayIndex = 0;
		barrier.MipIndex = 0;
		barrier.SourceAccessMask = GraphicsMemoryAccessFlags::ColorAttachmentRead;
		barrier.DestinationAccessMask = GraphicsMemoryAccessFlags::ColorAttachmentWrite;
		barrier.SourceQueue = GraphicsQueueType::Graphics;
		barrier.DestinationQueue = GraphicsQueueType::Graphics;
		barrier.OldLayout = TextureLayout::Present;
		barrier.NewLayout = TextureLayout::ColorAttachment;
		barrier.SourceStageFlags = PipelineStageFlags::ColorAttachmentOutput;
		barrier.DestinationStageFlags = PipelineStageFlags::ColorAttachmentOutput;

		barrier.AspectMask = TextureAspectFlags::ColorAspect;

		mCmdBuffer.lock()->SetTextureBarrier(image, barrier);

		DynamicPassAttachmentDesc passColorAttachmentDesc = {};
		passColorAttachmentDesc.ImageBuffer = mSwapchain.lock()->GetImageView(index);
		passColorAttachmentDesc.ImageLayout = TextureLayout::ColorAttachment;
		passColorAttachmentDesc.LoadOperation = AttachmentLoadOperation::Clear;
		passColorAttachmentDesc.StoreOperation = AttachmentStoreOperation::Store;
		passColorAttachmentDesc.ClearColor = { 0.1f, 0.2f, 0.3f, 1.0f };

		DynamicPassDesc passDesc = {};
		passDesc.ColorAttachments = { passColorAttachmentDesc };
		passDesc.layerCount = 1;
		passDesc.Extent = { mSwapchain.lock()->GetImageSize().x, mSwapchain.lock()->GetImageSize().y};
		passDesc.Offset = { 0, 0 };
		passDesc.viewMask = 0;

		mCmdBuffer.lock()->BeginRendering(passDesc);

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiBeginModule::Stop() noexcept
	{
	}

	bool ImGuiBeginModule::OnInitialize() noexcept
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

	void ImGuiBeginModule::OnPreUpdate() noexcept
	{
	}

	void ImGuiBeginModule::OnPostUpdate() noexcept
	{
	}
}
