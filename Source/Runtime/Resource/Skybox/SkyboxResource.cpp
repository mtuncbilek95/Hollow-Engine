#include "SkyboxResource.h"

#include <Runtime/Graphics/Texture/TextureUtils.h>
#include <Runtime/Graphics/API/GraphicsManager.h>

namespace Hollow
{
	SkyboxResource::SkyboxResource()
	{
		mGraphicsDevice = GraphicsManager::GetAPI().GetDefDevice();
		CreateInternalResources();
	}

	void SkyboxResource::ConnectMemory(const SharedPtr<GraphicsMemory>& hostMemory, const SharedPtr<GraphicsMemory>& deviceMemory, bool bPreAllocate)
	{
		mHostMemory = hostMemory;
		mDeviceMemory = deviceMemory;
		mPreAllocate = bPreAllocate;
	}

	void SkyboxResource::CreateTextureAndBuffer(const TextureDesc& desc, const TextureType viewType)
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
			stageDesc.SubSizeInBytes = mTexture->GetImageSize().x * mTexture->GetImageSize().y * 4 * 6;
			stageDesc.Usage = GraphicsBufferUsage::TransferSource;

			mStageBuffer = mGraphicsDevice->CreateGraphicsBuffer(stageDesc);
		}
	}

	void SkyboxResource::UpdateTextureAndBuffer(DArray<TextureResourceLayout> pBuffer, u32 offset)
	{
		u64 totalSize = 0;

		// Calculate the total size of the buffer that will be created for the skybox
		for(auto& layout : pBuffer)
			totalSize += layout.ImageData.GetSize();

		// Create a memory owned buffer to hold the skybox data
		void* pSkyboxData = malloc(totalSize);
		CORE_ASSERT(pSkyboxData, "Skybox Resource", "Failed to allocate memory for skybox data");

		// Copy the skybox data into the memory owned buffer
		u64 offsetInBytes = 0;
		for(auto& layout : pBuffer)
		{
			memcpy_s((u8*)pSkyboxData + (u64)offsetInBytes, totalSize, (u8*)layout.ImageData.GetData(), layout.ImageData.GetSize());
			offsetInBytes += layout.ImageData.GetSize();
		}

		// Create a memory owned buffer to hold the skybox data
		MemoryOwnedBuffer skyboxBuffer = { pSkyboxData, totalSize };

		if(pSkyboxData)
			free(pSkyboxData);

		BufferDataUpdateDesc textureDataUpdateDesc = {};
		textureDataUpdateDesc.Memory = skyboxBuffer;
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

	void SkyboxResource::CreateInternalResources()
	{
		CommandPoolDesc poolDesc = { CommandPoolType::Graphics };
		mCommandPool = mGraphicsDevice->CreateCommandPool(poolDesc);

		CommandBufferDesc bufferDesc = { mCommandPool };
		mCommandBuffer = mGraphicsDevice->CreateCommandBuffer(bufferDesc);

		FenceDesc fenceDesc = { false };
		mFence = mGraphicsDevice->CreateSyncFence(fenceDesc);
	}
}
