#include "TextureResource.h"

#include <Runtime/Graphics/Texture/TextureUtils.h>
#include <Runtime/Graphics/API/GraphicsManager.h>

namespace Hollow
{
	TextureResource::TextureResource(TextureResourceSubType type) : ResourceSubObject(), mSubType(type)
	{
		mGraphicsDevice = GraphicsManager::GetAPI().GetDefDevice();
		CreateInternalResources();
	}

	void TextureResource::ConnectMemory(const SharedPtr<GraphicsMemory>& hostMemory, const SharedPtr<GraphicsMemory>& deviceMemory, bool bPreAllocate)
	{
		mHostMemory = hostMemory;
		mDeviceMemory = deviceMemory;
		mPreAllocate = bPreAllocate;
	}

	void TextureResource::CreateTextureAndBuffer(const TextureDesc& desc, const TextureType viewType)
	{
		mTexture = mGraphicsDevice->CreateTexture(desc);

		TextureBufferDesc bufferDesc = {};
		bufferDesc.pTexture = mTexture;
		bufferDesc.Type = viewType;
		bufferDesc.ArrayLayer = 0;
		bufferDesc.ArrayCount = desc.ArraySize;
		bufferDesc.AspectFlags = TextureAspectFlags::ColorAspect;
		bufferDesc.MipLevel = 0;
		bufferDesc.MipCount = desc.MipLevels;

		mTextureBuffer = mGraphicsDevice->CreateTextureBuffer(bufferDesc);

		if (mPreAllocate)
		{
			GraphicsBufferDesc stageDesc = {};
			stageDesc.pMemory = mHostMemory;
			stageDesc.ShareMode = ShareMode::Exclusive;
			stageDesc.SubResourceCount = 1;
			stageDesc.SubSizeInBytes = mTexture->GetImageSize().x * mTexture->GetImageSize().y * 4;
			stageDesc.Usage = GraphicsBufferUsage::TransferSource;

			mStageBuffer = mGraphicsDevice->CreateGraphicsBuffer(stageDesc);
		}
	}

	void TextureResource::UpdateTextureAndBuffer(MemoryOwnedBuffer pBuffer, u32 offset)
	{
		BufferDataUpdateDesc textureDataUpdateDesc = {};
		textureDataUpdateDesc.Memory = pBuffer;
		textureDataUpdateDesc.OffsetInBytes = 0;
		mGraphicsDevice->UpdateBufferData(mStageBuffer, textureDataUpdateDesc);

		mCommandBuffer->BeginRecording();
		TextureBarrierUpdateDesc preTextureBarrier = {};
		preTextureBarrier.MipIndex = 0;
		preTextureBarrier.ArrayIndex = 0;
		preTextureBarrier.SourceAccessMask = GraphicsMemoryAccessFlags::Unknown;
		preTextureBarrier.OldLayout = TextureMemoryLayout::Unknown;
		preTextureBarrier.SourceQueue = GraphicsQueueType::Graphics;
		preTextureBarrier.SourceStageFlags = PipelineStageFlags::TopOfPipe;

		preTextureBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::TransferWrite;
		preTextureBarrier.NewLayout = TextureMemoryLayout::TransferDestination;
		preTextureBarrier.DestinationQueue = GraphicsQueueType::Graphics;
		preTextureBarrier.DestinationStageFlags = PipelineStageFlags::Transfer;

		preTextureBarrier.AspectMask = TextureAspectFlags::ColorAspect;

		mCommandBuffer->SetTextureBarrier(mTexture, preTextureBarrier);

		BufferTextureCopyDesc textureCopyDesc = {};
		textureCopyDesc.BufferOffsetInBytes = 0;
		textureCopyDesc.TargetArrayIndex = 0;
		textureCopyDesc.TargetMipIndex = 0;
		textureCopyDesc.TextureOffset = { 0,0,0 };
		textureCopyDesc.TextureSize = mTexture->GetImageSize();
		mCommandBuffer->CopyBufferToTexture(mStageBuffer, mTexture, textureCopyDesc);

		TextureBarrierUpdateDesc postTextureBarrier = {};
		postTextureBarrier.MipIndex = 0;
		postTextureBarrier.ArrayIndex = 0;
		postTextureBarrier.SourceAccessMask = GraphicsMemoryAccessFlags::TransferWrite;
		postTextureBarrier.OldLayout = TextureMemoryLayout::TransferDestination;
		postTextureBarrier.SourceQueue = GraphicsQueueType::Graphics;
		postTextureBarrier.SourceStageFlags = PipelineStageFlags::Transfer;

		postTextureBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::ShaderRead;
		postTextureBarrier.NewLayout = TextureMemoryLayout::ShaderReadOnly;
		postTextureBarrier.DestinationQueue = GraphicsQueueType::Graphics;
		postTextureBarrier.DestinationStageFlags = PipelineStageFlags::FragmentShader;

		postTextureBarrier.AspectMask = TextureAspectFlags::ColorAspect;

		mCommandBuffer->SetTextureBarrier(mTexture, postTextureBarrier);

		mCommandBuffer->EndRecording();
		mGraphicsDevice->SubmitToQueue(GraphicsManager::GetAPI().GetPresentQueue(), &mCommandBuffer, 1, nullptr, 0, nullptr, nullptr, 0, mFence);

		mGraphicsDevice->WaitForFence(&mFence, 1);
		mGraphicsDevice->ResetFences(&mFence, 1);
	}

	void TextureResource::CreateInternalResources()
	{
		CommandPoolDesc poolDesc = { CommandPoolType::Graphics };
		mCommandPool = mGraphicsDevice->CreateCommandPool(poolDesc);

		CommandBufferDesc bufferDesc = { mCommandPool };
		mCommandBuffer = mGraphicsDevice->CreateCommandBuffer(bufferDesc);

		FenceDesc fenceDesc = { false };
		mFence = mGraphicsDevice->CreateSyncFence(fenceDesc);
	}
}

