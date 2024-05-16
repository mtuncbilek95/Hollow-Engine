#include "RenderTargetResource.h"

#include <Runtime/Graphics/API/GraphicsManager.h>

namespace Hollow
{
	RenderTargetResource::RenderTargetResource() : ResourceSubObject(), mRenderRegionSize(0, 0)
	{
		mGraphicsDevice = GraphicsManager::GetInstanceAPI().GetDefaultDevice();
	}

	void RenderTargetResource::CreateRenderTarget(const RenderPassDesc& desc)
	{
		if (mRenderPass)
			ResetObject();

		mRenderPass = mGraphicsDevice->CreateRenderPass(desc);

		for (auto& colorDesc : desc.Attachments)
			mColorTextures.push_back(colorDesc.pTextureBuffer->GetTexture());

		if (desc.DepthStencilAttachment.pTextureBuffer)
			mDepthTextures = desc.DepthStencilAttachment.pTextureBuffer->GetTexture();
	}

	void RenderTargetResource::ResetObject() noexcept
	{
	}

	void RenderTargetResource::OnShutdown() noexcept
	{
	}
}
