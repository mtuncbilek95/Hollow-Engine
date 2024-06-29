#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Core/ResourceSubObject.h>
#include <Runtime/Resource/RenderTarget/RenderTargetDesc.h>
#include <Runtime/Resource/RenderTarget/RenderTargetBarrier.h>

#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Texture/TextureBuffer.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Graphics/Fence/Fence.h>

namespace Hollow
{
	class RUNTIME_API RenderTarget : public ResourceSubObject
	{
	public:
		RenderTarget();
		RenderTarget(const RenderTargetDesc& desc);
		virtual ~RenderTarget() override = default;

		SharedPtr<Texture> GetColorTexture() const { return mColorTexture; }
		SharedPtr<Texture> GetDepthTexture() const { return mDepthTexture; }

		SharedPtr<TextureBuffer> GetColorBuffer() const { return mColorBuffer; }
		SharedPtr<TextureBuffer> GetDepthBuffer() const { return mDepthBuffer; }
		
		virtual ResourceObjectType GetObjectType() const noexcept override { return ResourceObjectType::RenderTarget; }

		/* 
		 Every buffer starts with the unkown layout at top of pipe. This function help with the transition.
		 Everytime a new layout is set, current layout will be updated. So each time a new layout is set, the previous layout will be updated.
		 There is one job to do, which is to define the next layout for barrier.
		 */
		void NewLayoutForColor(const RenderTargetBarrier desc);
		/* 
		 Every buffer starts with the unkown layout at top of pipe. This function help with the transition. 
		 Everytime a new layout is set, current layout will be updated. So each time a new layout is set, the previous layout will be updated.
		 There is one job to do, which is to define the next layout for barrier.
		 */
		void NewLayoutForDepth(const RenderTargetBarrier desc);

	private:
		void CreateInternalResources();

	private:
		SharedPtr<Texture> mColorTexture;
		SharedPtr<Texture> mDepthTexture;

		SharedPtr<TextureBuffer> mColorBuffer;
		SharedPtr<TextureBuffer> mDepthBuffer;

		SharedPtr<GraphicsDevice> mGraphicsDevice;
		SharedPtr<GraphicsMemory> mDeviceMemory;

		SharedPtr<CommandBuffer> mCommandBuffer;
		SharedPtr<CommandPool> mCommandPool;

		SharedPtr<Fence> mFence;

		TextureFormat mColorFormat;
		TextureFormat mDepthFormat;
		TextureType mTargetType;
		TextureSampleCount mSampleCount;
		TextureUsage mColorUsage;
		TextureUsage mDepthUsage;

		Vec2u mImageSize;

		RenderTargetBarrier mColorBarrier;
		RenderTargetBarrier mDepthBarrier;
	};
}
