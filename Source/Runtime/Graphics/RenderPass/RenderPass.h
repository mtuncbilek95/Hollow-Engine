#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/RenderPass/RenderPassDesc.h>

namespace Hollow
{
	class RUNTIME_API RenderPass : public GraphicsDeviceObject
	{
	public:
		RenderPass(const RenderPassDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice) , mTargetRenderSize(desc.TargetSize), 
			mColorAttachments(desc.Attachments), mDepthStencilAttachment(desc.DepthStencilAttachment)
		{}
		virtual ~RenderPass() override = default;

		const Vector2u& GetTargetRendersize() const noexcept { return mTargetRenderSize; }
		const arrayList<RenderPassAttachmentDesc>& GetColorAttachments() const noexcept { return mColorAttachments; }
		const RenderPassAttachmentDesc& GetDepthStencilAttachment() const noexcept { return mDepthStencilAttachment; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::RenderPass; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		Vector2u mTargetRenderSize;
		arrayList<RenderPassAttachmentDesc> mColorAttachments;
		RenderPassAttachmentDesc mDepthStencilAttachment;
	};
}