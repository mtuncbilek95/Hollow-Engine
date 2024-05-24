#include "RenderTargetResource.h"

#include <Runtime/Graphics/API/GraphicsManager.h>

namespace Hollow
{
	SharedPtr<RenderTargetResource> RenderTargetResource::CreateRenderTarget(const RenderTargetDesc& desc)
	{
		auto renderTarget = std::make_shared<RenderTargetResource>();
		if (desc.DescStruct.ColorAttachmentCount > 0)
		{
			renderTarget->CreateColorBuffers(desc);
			renderTarget->CreateDepthBuffer(desc);
			renderTarget->CreateStencilBuffer(desc);
		}
		else
		{
			renderTarget->ConnectColorBuffer(desc);
			renderTarget->ConnectDepthBuffer(desc);
			renderTarget->ConnectStencilBuffer(desc);
		}
		return renderTarget;
	}

	RenderTargetResource::RenderTargetResource()
	{
		mGraphicsDevice = GraphicsManager::GetInstanceAPI().GetDefaultDevice();
	}

	void RenderTargetResource::OnShutdown() noexcept
	{
	}

	void RenderTargetResource::CreateColorBuffers(const RenderTargetDesc& desc)
	{
		for (uint32 i = 0; i < desc.DescStruct.ColorAttachmentCount; i++)
		{
			auto texture = mGraphicsDevice->CreateTexture(desc.DescStruct.ColorAttachment);

			TextureBufferDesc bufferDesc = {};
			bufferDesc.ArrayLayer = 0;
			bufferDesc.MipLevel = 0;
			bufferDesc.AspectFlags = TextureAspectFlags::ColorAspect;
			bufferDesc.pTexture = texture;

			auto buffer = mGraphicsDevice->CreateTextureBuffer(bufferDesc);
			mColorBuffers.push_back(buffer);
		}
	}

	void RenderTargetResource::CreateDepthBuffer(const RenderTargetDesc& desc)
	{
		if (!desc.DescStruct.HasDepthAttachment)
			return;

		// Create depth texture
		auto texture = mGraphicsDevice->CreateTexture(desc.DescStruct.DepthAttachment);

		// Create depth buffer
		TextureBufferDesc bufferDesc = {};
		bufferDesc.ArrayLayer = 0;
		bufferDesc.MipLevel = 0;
		bufferDesc.AspectFlags = TextureAspectFlags::DepthAspect;
		bufferDesc.pTexture = texture;
		mDepthBuffer = mGraphicsDevice->CreateTextureBuffer(bufferDesc);
	}

	void RenderTargetResource::CreateStencilBuffer(const RenderTargetDesc& desc)
	{
		if (!desc.DescStruct.HasStencilAttachment)
			return;
	}
	void RenderTargetResource::ConnectColorBuffer(const RenderTargetDesc& desc)
	{
		for (uint32 i = 0; i < desc.AttachmentStruct.ColorAttachments.size(); i++)
		{
				TextureBufferDesc bufferDesc = {};
				bufferDesc.ArrayLayer = 0;
				bufferDesc.MipLevel = 0;
				bufferDesc.AspectFlags = TextureAspectFlags::ColorAspect;
				bufferDesc.pTexture = desc.AttachmentStruct.ColorAttachments[i];

				auto buffer = mGraphicsDevice->CreateTextureBuffer(bufferDesc);
				mColorBuffers.push_back(buffer);
		}
	}

	void RenderTargetResource::ConnectDepthBuffer(const RenderTargetDesc& desc)
	{
		if (!desc.AttachmentStruct.DepthAttachment)
			return;

		TextureBufferDesc bufferDesc = {};
		bufferDesc.ArrayLayer = 0;
		bufferDesc.MipLevel = 0;
		bufferDesc.AspectFlags = TextureAspectFlags::DepthAspect;
		bufferDesc.pTexture = desc.AttachmentStruct.DepthAttachment;

		mDepthBuffer = mGraphicsDevice->CreateTextureBuffer(bufferDesc);
	}

	void RenderTargetResource::ConnectStencilBuffer(const RenderTargetDesc& desc)
	{
		if (!desc.AttachmentStruct.StencilAttachment)
			return;
	}
}
