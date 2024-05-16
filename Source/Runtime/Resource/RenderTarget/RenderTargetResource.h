#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Resource/Core/ResourceSubObject.h>

namespace Hollow
{
	class RUNTIME_API RenderTargetResource : public ResourceSubObject
	{
	public:
		RenderTargetResource();
		virtual ~RenderTargetResource() override = default;

		const ArrayList<SharedPtr<Texture>>& GetColorTextures() const noexcept { return mColorTextures; }
		const SharedPtr<Texture> GetDepthTexture() const noexcept { return mDepthTextures; }
		const SharedPtr<RenderPass> GetRenderPass() const noexcept { return mRenderPass; }
		const Vector2u GetRenderRegionSize() const noexcept { return mRenderRegionSize; }

		void CreateRenderTarget(const RenderPassDesc& desc);

		virtual void ResetObject() noexcept override;
		virtual void OnShutdown() noexcept override;
		virtual ResourceObjectType GetObjectType() const noexcept final { return ResourceObjectType::RenderTarget; }

	private:
		SharedPtr<GraphicsDevice> mGraphicsDevice;
		ArrayList<SharedPtr<Texture>> mColorTextures;
		SharedPtr<Texture> mDepthTextures;

		SharedPtr<RenderPass> mRenderPass;
		Vector2u mRenderRegionSize;
	};
}
