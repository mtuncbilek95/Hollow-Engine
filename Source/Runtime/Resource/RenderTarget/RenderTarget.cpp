#include "RenderTarget.h"

#include <Runtime/Graphics/API/GraphicsManager.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>

namespace Hollow
{
	RenderTarget::RenderTarget() : ResourceSubObject()
	{
		CreateInternalResources();

		TextureDesc colorTextureDesc = {};
		colorTextureDesc.ArraySize = 1;
		colorTextureDesc.ImageFormat = mGraphicsDevice->GetSwapchain()->GetSwapchainFormat();
		colorTextureDesc.ImageSize = { mGraphicsDevice->GetSwapchain()->GetImageSize().x, mGraphicsDevice->GetSwapchain()->GetImageSize().y, 1 };
		colorTextureDesc.MipLevels = 1;
		colorTextureDesc.SampleCount = static_cast<TextureSampleCount>(GraphicsManager::GetAPI().GetMsaaSamples());
		colorTextureDesc.Type = (int)colorTextureDesc.SampleCount > 1 ? TextureType::Texture2DMS : TextureType::Texture2D;
		colorTextureDesc.Usage = TextureUsage::ColorAttachment;
		colorTextureDesc.pMemory = mDeviceMemory;

		mColorTexture = mGraphicsDevice->CreateTexture(colorTextureDesc);

		TextureBufferDesc colorBufferDesc = {};
		colorBufferDesc.pTexture = mColorTexture;
		colorBufferDesc.ArrayLayer = 0;
		colorBufferDesc.AspectFlags = TextureAspectFlags::ColorAspect;
		colorBufferDesc.MipLevel = 0;

		mColorBuffer = mGraphicsDevice->CreateTextureBuffer(colorBufferDesc);

		TextureDesc depthTextureDesc = {};
		depthTextureDesc.ArraySize = 1;
		depthTextureDesc.ImageFormat = TextureFormat::D32_Float;
		depthTextureDesc.ImageSize = { mGraphicsDevice->GetSwapchain()->GetImageSize().x, mGraphicsDevice->GetSwapchain()->GetImageSize().y, 1 };
		depthTextureDesc.MipLevels = 1;
		depthTextureDesc.SampleCount = static_cast<TextureSampleCount>(GraphicsManager::GetAPI().GetMsaaSamples());
		depthTextureDesc.Type = (int)depthTextureDesc.SampleCount > 1 ? TextureType::Texture2DMS : TextureType::Texture2D;
		depthTextureDesc.Usage = TextureUsage::DepthStencilAttachment;
		depthTextureDesc.pMemory = mDeviceMemory;

		mDepthTexture = mGraphicsDevice->CreateTexture(depthTextureDesc);

		TextureBufferDesc depthBufferDesc = {};
		depthBufferDesc.pTexture = mDepthTexture;
		depthBufferDesc.ArrayLayer = 0;
		depthBufferDesc.AspectFlags = TextureAspectFlags::DepthAspect;
		depthBufferDesc.MipLevel = 0;

		mDepthBuffer = mGraphicsDevice->CreateTextureBuffer(depthBufferDesc);

		mColorBarrier = {};
		mColorBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::Unknown;
		mColorBarrier.DestinationQueue = GraphicsQueueType::Graphics;
		mColorBarrier.NewLayout = TextureMemoryLayout::Unknown;
		mColorBarrier.DestinationStageFlags = PipelineStageFlags::TopOfPipe;
		mColorBarrier.AspectMask = TextureAspectFlags::ColorAspect;

		mDepthBarrier = {};
		mDepthBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::Unknown;
		mDepthBarrier.DestinationQueue = GraphicsQueueType::Graphics;
		mDepthBarrier.NewLayout = TextureMemoryLayout::Unknown;
		mDepthBarrier.DestinationStageFlags = PipelineStageFlags::TopOfPipe;
		mDepthBarrier.AspectMask = TextureAspectFlags::DepthAspect;
	}

	RenderTarget::RenderTarget(const RenderTargetDesc& desc) : ResourceSubObject(), mColorFormat(desc.ColorFormat), 
		mDepthFormat(desc.DepthFormat), mTargetType(desc.TargetType), mSampleCount(desc.SampleCount), mColorUsage(desc.ColorUsage), 
		mDepthUsage(desc.DepthUsage), mImageSize(desc.ImageSize)
	{
		CreateInternalResources();

		TextureDesc colorTextureDesc = {};
		colorTextureDesc.ArraySize = 1;
		colorTextureDesc.ImageFormat = desc.ColorFormat;
		colorTextureDesc.ImageSize = { desc.ImageSize.x, desc.ImageSize.y, 1 };
		colorTextureDesc.MipLevels = 1;
		colorTextureDesc.SampleCount = desc.SampleCount;
		colorTextureDesc.Type = desc.TargetType;
		colorTextureDesc.Usage = desc.ColorUsage;
		colorTextureDesc.pMemory = mDeviceMemory;

		mColorTexture = mGraphicsDevice->CreateTexture(colorTextureDesc);

		TextureBufferDesc colorBufferDesc = {};
		colorBufferDesc.pTexture = mColorTexture;
		colorBufferDesc.ArrayLayer = 0;
		colorBufferDesc.AspectFlags = TextureAspectFlags::ColorAspect;
		colorBufferDesc.MipLevel = 0;

		mColorBuffer = mGraphicsDevice->CreateTextureBuffer(colorBufferDesc);

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
		depthBufferDesc.AspectFlags = TextureAspectFlags::DepthAspect;
		depthBufferDesc.MipLevel = 0;

		mDepthBuffer = mGraphicsDevice->CreateTextureBuffer(depthBufferDesc);

		mColorBarrier = {};
		mColorBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::Unknown;
		mColorBarrier.DestinationQueue = GraphicsQueueType::Graphics;
		mColorBarrier.NewLayout = TextureMemoryLayout::Unknown;
		mColorBarrier.DestinationStageFlags = PipelineStageFlags::TopOfPipe;
		mColorBarrier.AspectMask = TextureAspectFlags::ColorAspect;

		mDepthBarrier = {};
		mDepthBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::Unknown;
		mDepthBarrier.DestinationQueue = GraphicsQueueType::Graphics;
		mDepthBarrier.NewLayout = TextureMemoryLayout::Unknown;
		mDepthBarrier.DestinationStageFlags = PipelineStageFlags::TopOfPipe;
		mDepthBarrier.AspectMask = TextureAspectFlags::DepthAspect;
	}

	void RenderTarget::NewLayoutForColor(const RenderTargetBarrier desc)
	{
		TextureBarrierUpdateDesc colorBarrier = {};
		colorBarrier.MipIndex = 0;
		colorBarrier.ArrayIndex = 0;
		colorBarrier.SourceAccessMask = mColorBarrier.DestinationAccessMask;
		colorBarrier.SourceQueue = mColorBarrier.DestinationQueue;
		colorBarrier.OldLayout = mColorBarrier.NewLayout;
		colorBarrier.SourceStageFlags = mColorBarrier.DestinationStageFlags;

		colorBarrier.DestinationAccessMask = desc.DestinationAccessMask;
		colorBarrier.DestinationQueue = desc.DestinationQueue;
		colorBarrier.NewLayout = desc.NewLayout;
		colorBarrier.DestinationStageFlags = desc.DestinationStageFlags;

		colorBarrier.AspectMask = desc.AspectMask;

		mCommandBuffer->BeginRecording();
		mCommandBuffer->SetTextureBarrier(mColorTexture, colorBarrier);
		mCommandBuffer->EndRecording();

		mGraphicsDevice->SubmitToQueue(GraphicsManager::GetAPI().GetPresentQueue(), &mCommandBuffer, 1, nullptr, 0, nullptr, nullptr, 0, mFence);

		mGraphicsDevice->WaitForFence(&mFence, 1);
		mGraphicsDevice->ResetFences(&mFence, 1);

		mColorBarrier = desc;
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
