#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/Object.h>
#include <Runtime/Graphics/API/GraphicsManager.h>

#include <imgui.h>

namespace Hollow
{
	class RUNTIME_API ImGuiRenderer : public Object
	{
	public:
		ImGuiRenderer();
		virtual ~ImGuiRenderer() override = default;

		virtual void OnShutdown() override;
	private:
		ImGuiContext* mContext;

		SharedPtr<GraphicsDevice> mDevice;
		SharedPtr<Swapchain> mSwapchain;
		SharedPtr<GraphicsQueue> mGraphicsQueue;
		SharedPtr<RenderPass> mRenderPass;

		SharedPtr<Pipeline> mImguiPipeline;
		SharedPtr<DescriptorSet> mDescriptorSet;
		SharedPtr<DescriptorPool> mDescriptorPool;
		SharedPtr<DescriptorLayout> mDescriptorLayout;

		SharedPtr<GraphicsBuffer> mVertexBuffer;
		SharedPtr<GraphicsBuffer> mIndexBuffer;
		SharedPtr<GraphicsBuffer> mUniformBuffer;

		SharedPtr<GraphicsBuffer> mVertexStagingBuffer;
		SharedPtr<GraphicsBuffer> mIndexStagingBuffer;
		SharedPtr<GraphicsBuffer> mUniformStagingBuffer;

		SharedPtr<Fence> mFence;

	};
}
