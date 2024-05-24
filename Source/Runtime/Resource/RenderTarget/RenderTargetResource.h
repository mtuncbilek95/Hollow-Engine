#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Core/ResourceSubObject.h>
#include <Runtime/Resource/RenderTarget/RenderTargetDesc.h>
#include <Runtime/Graphics/Texture/TextureBuffer.h>

namespace Hollow
{
	class RUNTIME_API RenderTargetResource : public ResourceSubObject
	{
	public:
		static SharedPtr<RenderTargetResource> CreateRenderTarget(const RenderTargetDesc& desc);
	public:
		RenderTargetResource();
		virtual ~RenderTargetResource() override = default;

		ArrayList<SharedPtr<TextureBuffer>>& GetColorBuffers() { return mColorBuffers; }
		SharedPtr<TextureBuffer> GetColorBuffer(uint32 index) { return mColorBuffers[index]; }
		SharedPtr<TextureBuffer> GetDepthBuffer() { return mDepthBuffer; }
		SharedPtr<TextureBuffer> GetStencilBuffer() { return mStencilBuffer; }

		Vector2u GetRegionSize() const { return mRegionSize; }

		virtual void OnShutdown() noexcept override;
		virtual ResourceObjectType GetObjectType() const noexcept { return ResourceObjectType::RenderTarget; }

	private:
		void CreateColorBuffers(const RenderTargetDesc& desc);
		void CreateDepthBuffer(const RenderTargetDesc& desc);
		void CreateStencilBuffer(const RenderTargetDesc& desc);

		void ConnectColorBuffer(const RenderTargetDesc& desc);
		void ConnectDepthBuffer(const RenderTargetDesc& desc);
		void ConnectStencilBuffer(const RenderTargetDesc& desc);

	private:
		ArrayList<SharedPtr<TextureBuffer>> mColorBuffers;
		SharedPtr<TextureBuffer> mDepthBuffer;
		SharedPtr<TextureBuffer> mStencilBuffer;
		Vector2u mRegionSize;

		SharedPtr<GraphicsDevice> mGraphicsDevice;
	};
}
