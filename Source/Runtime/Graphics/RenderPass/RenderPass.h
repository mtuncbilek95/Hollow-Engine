#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/RenderPass/RenderPassDesc.h>

namespace Hollow
{
	class RUNTIME_API RenderPass : public GraphicsDeviceObject
	{
	public:
		RenderPass(const RenderPassDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), 
			mFramebufferSize(desc.FramebufferSize), 
			mColorAttachments(desc.ColorAttachments), mDepthStencilAttachment(desc.DepthStencilAttachment), 
			mIsSwapchainTarget(desc.IsSwapchainTarget) 
		{}
		virtual ~RenderPass() override = default;

		const Vector2u& GetFramebufferSize() const { return mFramebufferSize; }
		const Array<RenderPassAttachmentDesc>& GetColorAttachments() const { return mColorAttachments; }
		const RenderPassAttachmentDesc& GetDepthStencilAttachment() const { return mDepthStencilAttachment; }
		bool IsSwapchainTarget() const { return mIsSwapchainTarget; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept override final { return GraphicsDeviceObjectType::RenderPass; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		Vector2u mFramebufferSize;
		Array<RenderPassAttachmentDesc> mColorAttachments;
		RenderPassAttachmentDesc mDepthStencilAttachment;
		bool mIsSwapchainTarget;
	};
}
