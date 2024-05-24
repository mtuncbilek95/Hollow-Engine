#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Framebuffer/FramebufferDesc.h>

namespace Hollow
{
	class RUNTIME_API Framebuffer : public GraphicsDeviceObject
	{
	public:
		Framebuffer(const FramebufferDesc& desc, SharedPtr<GraphicsDevice> device) : GraphicsDeviceObject(device), 
			mRenderPass(desc.RPass), mAttachments(desc.Attachments), mImageSize(desc.ImageSize) {}
		virtual ~Framebuffer() override = default;

		FORCEINLINE SharedPtr<RenderPass> GetRenderPass() const noexcept { return mRenderPass; }
		FORCEINLINE const ArrayList<SharedPtr<TextureBuffer>>& GetAttachments() const noexcept { return mAttachments; }
		FORCEINLINE SharedPtr<TextureBuffer> GetAttachment(uint32 index) const noexcept { return mAttachments[index]; }
		FORCEINLINE const Vector2u& GetImageSize() const noexcept { return mImageSize; }

		FORCEINLINE virtual GraphicsDeviceObjectType GetObjectType() const noexcept { return GraphicsDeviceObjectType::Framebuffer; }

		virtual void OnShutdown() override = 0;

	private:
		SharedPtr<RenderPass> mRenderPass;
		ArrayList<SharedPtr<TextureBuffer>> mAttachments;
		Vector2u mImageSize;
	};
}
