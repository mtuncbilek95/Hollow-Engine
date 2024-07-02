#include "RenderTarget.h"

#include <Runtime/Graphics/API/GraphicsManager.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>

namespace Hollow
{
	RenderTarget::RenderTarget(const RenderTargetDesc& desc) : ResourceSubObject(), mColorFormat(desc.ColorFormat),
		mDepthFormat(desc.DepthFormat), mTargetType(desc.TargetType), mSampleCount(desc.SampleCount), mColorUsage(desc.ColorUsage),
		mDepthUsage(desc.DepthUsage), mImageSize(desc.ImageSize)
	{
		CreateInternalResources();

		if (desc.ColorAttachmentCount > 0)
		{
			for (u32 i = 0; i < desc.ColorAttachmentCount; i++)
			{
				TextureDesc colorTextureDesc = {};
				colorTextureDesc.ArraySize = 1;
				colorTextureDesc.ImageFormat = desc.ColorFormat;
				colorTextureDesc.ImageSize = { desc.ImageSize.x, desc.ImageSize.y, 1 };
				colorTextureDesc.MipLevels = 1;
				colorTextureDesc.SampleCount = desc.SampleCount;
				colorTextureDesc.Type = desc.TargetType;
				colorTextureDesc.Usage = desc.ColorUsage;
				colorTextureDesc.pMemory = mDeviceMemory;

				mColorTextures.push_back(mGraphicsDevice->CreateTexture(colorTextureDesc));

				TextureBufferDesc colorBufferDesc = {};
				colorBufferDesc.pTexture = mColorTextures[0];
				colorBufferDesc.ArrayLayer = 0;
				colorBufferDesc.ArrayCount = 1;
				colorBufferDesc.AspectFlags = TextureAspectFlags::ColorAspect;
				colorBufferDesc.MipLevel = 0;
				colorBufferDesc.MipCount = 1;
				colorBufferDesc.Type = desc.TargetType;

				mColorBuffers.push_back(mGraphicsDevice->CreateTextureBuffer(colorBufferDesc));

				RenderTargetBarrier colorBarrier = {};
				colorBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::Unknown;
				colorBarrier.DestinationQueue = GraphicsQueueType::Graphics;
				colorBarrier.NewLayout = TextureMemoryLayout::Unknown;
				colorBarrier.DestinationStageFlags = PipelineStageFlags::TopOfPipe;
				colorBarrier.AspectMask = TextureAspectFlags::ColorAspect;

				mColorBarriers.push_back(colorBarrier);
			}

			if (desc.HasDepthTexture)
			{
				TextureDesc depthTextureDesc = {};
				depthTextureDesc.ArraySize = 1;
				depthTextureDesc.ImageFormat = desc.DepthFormat;
				depthTextureDesc.ImageSize = { desc.ImageSize.x, desc.ImageSize.y, 1 };
				depthTextureDesc.MipLevels = 1;
				depthTextureDesc.SampleCount = desc.SampleCount;
				depthTextureDesc.Type = desc.TargetType;
				depthTextureDesc.Usage = desc.DepthUsage;
				depthTextureDesc.pMemory = mDeviceMemory;

				mDepthTexture = mGraphicsDevice->CreateTexture(depthTextureDesc);

				TextureBufferDesc depthBufferDesc = {};
				depthBufferDesc.pTexture = mDepthTexture;
				depthBufferDesc.ArrayLayer = 0;
				depthBufferDesc.ArrayCount = 1;
				depthBufferDesc.AspectFlags = TextureAspectFlags::DepthAspect;
				depthBufferDesc.Type = desc.TargetType;
				depthBufferDesc.MipLevel = 0;
				depthBufferDesc.MipCount = 1;

				mDepthBuffer = mGraphicsDevice->CreateTextureBuffer(depthBufferDesc);

				RenderTargetBarrier depthBarrier = {};
				depthBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::Unknown;
				depthBarrier.DestinationQueue = GraphicsQueueType::Graphics;
				depthBarrier.NewLayout = TextureMemoryLayout::Unknown;
				depthBarrier.DestinationStageFlags = PipelineStageFlags::TopOfPipe;
				depthBarrier.AspectMask = TextureAspectFlags::DepthAspect;

				mDepthBarrier = depthBarrier;
			}
		}
		else
		{
			mColorTextures = desc.pColorTextures;
			mColorBuffers = desc.pColorTextureBuffers;

			for (i32 i = 0; i < mColorTextures.size(); i++)
			{
				RenderTargetBarrier colorBarrier = {};
				colorBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::Unknown;
				colorBarrier.DestinationQueue = GraphicsQueueType::Graphics;
				colorBarrier.NewLayout = TextureMemoryLayout::Unknown;
				colorBarrier.DestinationStageFlags = PipelineStageFlags::TopOfPipe;
				colorBarrier.AspectMask = TextureAspectFlags::ColorAspect;

				mColorBarriers.push_back(colorBarrier);
			}
		}
	}

	void RenderTarget::NewLayoutForColor(const RenderTargetBarrier desc, u32 texIndex)
	{
		TextureBarrierUpdateDesc colorBarrier = {};
		colorBarrier.MipIndex = 0;
		colorBarrier.ArrayIndex = 0;
		colorBarrier.SourceAccessMask = mColorBarriers[texIndex].DestinationAccessMask;
		colorBarrier.SourceQueue = mColorBarriers[texIndex].DestinationQueue;
		colorBarrier.OldLayout = mColorBarriers[texIndex].NewLayout;
		colorBarrier.SourceStageFlags = mColorBarriers[texIndex].DestinationStageFlags;

		colorBarrier.DestinationAccessMask = desc.DestinationAccessMask;
		colorBarrier.DestinationQueue = desc.DestinationQueue;
		colorBarrier.NewLayout = desc.NewLayout;
		colorBarrier.DestinationStageFlags = desc.DestinationStageFlags;

		colorBarrier.AspectMask = desc.AspectMask;

		mCommandBuffer->BeginRecording();
		mCommandBuffer->SetTextureBarrier(mColorTextures[texIndex], colorBarrier);
		mCommandBuffer->EndRecording();

		mGraphicsDevice->SubmitToQueue(GraphicsManager::GetAPI().GetPresentQueue(), &mCommandBuffer, 1, nullptr, 0, nullptr, nullptr, 0, mFence);

		mGraphicsDevice->WaitForFence(&mFence, 1);
		mGraphicsDevice->ResetFences(&mFence, 1);

		mColorBarriers[texIndex] = desc;
	}

	void RenderTarget::NewLayoutForDepth(const RenderTargetBarrier desc)
	{
		TextureBarrierUpdateDesc depthBarrier = {};
		depthBarrier.MipIndex = 0;
		depthBarrier.ArrayIndex = 0;
		depthBarrier.SourceAccessMask = mDepthBarrier.DestinationAccessMask;
		depthBarrier.SourceQueue = mDepthBarrier.DestinationQueue;
		depthBarrier.OldLayout = mDepthBarrier.NewLayout;
		depthBarrier.SourceStageFlags = mDepthBarrier.DestinationStageFlags;

		depthBarrier.DestinationAccessMask = desc.DestinationAccessMask;
		depthBarrier.DestinationQueue = desc.DestinationQueue;
		depthBarrier.NewLayout = desc.NewLayout;
		depthBarrier.DestinationStageFlags = desc.DestinationStageFlags;

		depthBarrier.AspectMask = desc.AspectMask;

		mCommandBuffer->BeginRecording();
		mCommandBuffer->SetTextureBarrier(mDepthTexture, depthBarrier);
		mCommandBuffer->EndRecording();

		mGraphicsDevice->SubmitToQueue(GraphicsManager::GetAPI().GetPresentQueue(), &mCommandBuffer, 1, nullptr, 0, nullptr, nullptr, 0, mFence);

		mGraphicsDevice->WaitForFence(&mFence, 1);
		mGraphicsDevice->ResetFences(&mFence, 1);

		mDepthBarrier = desc;
	}

	void RenderTarget::CreateInternalResources()
	{
		mGraphicsDevice = GraphicsManager::GetAPI().GetDefDevice();

		CommandPoolDesc poolDesc = { CommandPoolType::Graphics };
		mCommandPool = mGraphicsDevice->CreateCommandPool(poolDesc);

		CommandBufferDesc bufferDesc = { mCommandPool };
		mCommandBuffer = mGraphicsDevice->CreateCommandBuffer(bufferDesc);

		FenceDesc fenceDesc = { false };
		mFence = mGraphicsDevice->CreateSyncFence(fenceDesc);

		mDeviceMemory = GraphicsManager::GetAPI().GetDeviceMemory();
	}
}
