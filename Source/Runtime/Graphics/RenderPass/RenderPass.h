#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/RenderPass/RenderPassDesc.h>

namespace Hollow
{
	class RUNTIME_API RenderPass : public GraphicsDeviceObject
	{
	public:
		RenderPass(const RenderPassDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), mColorAttachments(desc.ColorAttachments), 
			mDepthStencilAttachment(desc.pDepthStencilAttachment), mRenderTargetSize(desc.TargetRenderSize) 
		{}
		virtual ~RenderPass() override = default;

		const Array<RenderPassAttachmentDesc>& GetColorAttachments() const { return mColorAttachments; }
		RenderPassAttachmentDesc* GetDepthStencilAttachment() const { return mDepthStencilAttachment; }
		const Vector2u& GetRenderTargetSize() const { return mRenderTargetSize; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::RenderPass; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		Array<RenderPassAttachmentDesc> mColorAttachments;
		RenderPassAttachmentDesc* mDepthStencilAttachment;
		Vector2u mRenderTargetSize;
	};
}