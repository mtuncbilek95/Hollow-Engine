#include "TextureResource.h"

#include <Runtime/Graphics/Texture/TextureUtils.h>
#include <Runtime/Graphics/API/GraphicsManager.h>

namespace Hollow
{
	TextureResource::TextureResource() : ResourceSubObject(), mMipDataInitialized(false)
	{
		mGraphicsDevice = GraphicsManager::GetInstanceAPI().GetDefaultDevice();
		CreateInternalResources();
	}

	SharedPtr<TextureBuffer> TextureResource::CreateResourceBuffer(const TextureAspectFlags flag, const byte mipLevel, const byte arrayLevel)
	{
		if (mDesc.ArraySize < arrayLevel || mDesc.MipLevels < mipLevel)
		{
			CORE_LOG(HE_WARNING, "TextureResource", "Invalid mip or array level");
			return nullptr;
		}

		auto& mipData = mMipData[arrayLevel][mipLevel];
		if (!mipData.MipTextureBuffer)
		{
			TextureBufferDesc desc = {};
			desc.AspectFlags = flag;
			desc.pTexture = mTexture;
			desc.MipLevel = mipLevel;
			desc.ArrayLayer = arrayLevel;

			mipData.MipTextureBuffer = mGraphicsDevice->CreateTextureBuffer(desc);
		}

		return mipData.MipTextureBuffer;
	}

	void TextureResource::InitializeMipData(const SharedPtr<Texture> texture)
	{
		if (mTexture)
			ResetObject();

		mTexture = texture;

		if (!mMipDataInitialized)
		{
			mMipData.resize(mTexture->GetArraySize());
			for (auto& mip : mMipData)
			{
				mip.resize(mTexture->GetMipLevels());
			}

			mMipDataInitialized = true;
		}
	}

	void TextureResource::ConnectMemory(const SharedPtr<GraphicsMemory> hostMemory, const SharedPtr<GraphicsMemory> deviceMemory)
	{
		mHostMemory = hostMemory;
		mDeviceMemory = deviceMemory;
	}

	void TextureResource::CreateTextureData(const TextureDesc& desc, bool preAllocMips, bool preAllocData)
	{
		mDesc = desc;

		mTexture = mGraphicsDevice->CreateTexture(desc);

		// For each arrayindex, create a mip chain
		for (uint32 arrayIndex = 0; arrayIndex <desc.ArraySize; ++arrayIndex)
		{
			ArrayList<MipData> mips;

			// For each mip level, create a mip data
			for (uint32 mipIndex = 0; mipIndex < desc.MipLevels; mipIndex++)
			{
				MipData mipData = {};

				if (preAllocData)
				{
					GraphicsBufferDesc bufferDesc = {};
					bufferDesc.pMemory = mHostMemory;
					bufferDesc.ShareMode = ShareMode::Exclusive;
					bufferDesc.SubResourceCount = 1;
					bufferDesc.SubSizeInBytes = mTexture->GetImageSize().x * mTexture->GetImageSize().y * TextureUtils::GetTextureFormatSize(mTexture->GetImageFormat());
					bufferDesc.Usage = GraphicsBufferUsage::TransferSource;

					mipData.MipStageBuffer = mGraphicsDevice->CreateGraphicsBuffer(bufferDesc);
				}

				if (preAllocMips)
				{
					TextureBufferDesc bufferDesc = {};
					bufferDesc.AspectFlags = TextureAspectFlags::ColorAspect;
					bufferDesc.ArrayLayer = arrayIndex;
					bufferDesc.MipLevel = mipIndex;
					bufferDesc.pTexture = mTexture;
					
					mipData.MipTextureBuffer = mGraphicsDevice->CreateTextureBuffer(bufferDesc);
				}

				mips.push_back(mipData);
			}

			mMipData.push_back(mips);
		}
	}

	void TextureResource::UpdateTextureData(const MemoryBuffer& data, const Vector3u& offset, const TextureMemoryLayout inputMemoryLayout, const GraphicsMemoryAccessFlags inputAccessFlags, const PipelineStageFlags inputPipelineFlags, const GraphicsQueueType inputQueueType, const byte mipLevel, const byte arrayLevel)
	{
		// First Update the staging buffer
		auto& mipData = mMipData[arrayLevel][mipLevel];

		if (mipData.MipStageBuffer)
		{
			BufferDataUpdateDesc updateDesc = {};
			updateDesc.Memory = data;
			updateDesc.OffsetInBytes = 0;
			mGraphicsDevice->UpdateBufferData(mipData.MipStageBuffer, updateDesc);
		}
		
		// Start recording the command buffer
		mCommandBuffer->BeginRecording();

		// Make sure that texture is ready to be written
		TextureBarrierUpdateDesc preTextureBarrier = {};
		preTextureBarrier.MipIndex = mipLevel;
		preTextureBarrier.ArrayIndex = arrayLevel;
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

		TextureBarrierUpdateDesc postTextureBarrier = {};
		postTextureBarrier.MipIndex = mipLevel;
		postTextureBarrier.ArrayIndex = arrayLevel;
		postTextureBarrier.SourceAccessMask = GraphicsMemoryAccessFlags::TransferWrite;
		postTextureBarrier.OldLayout = TextureMemoryLayout::TransferDestination;
		postTextureBarrier.SourceQueue = GraphicsQueueType::Graphics;
		postTextureBarrier.SourceStageFlags = PipelineStageFlags::Transfer;

		postTextureBarrier.DestinationAccessMask = inputAccessFlags;
		postTextureBarrier.NewLayout = inputMemoryLayout;
		postTextureBarrier.DestinationQueue = inputQueueType;
		postTextureBarrier.DestinationStageFlags = inputPipelineFlags;

		postTextureBarrier.AspectMask = TextureAspectFlags::ColorAspect;

		mCommandBuffer->SetTextureBarrier(mTexture, postTextureBarrier);

		mCommandBuffer->EndRecording();

		mGraphicsDevice->SubmitToQueue(GraphicsManager::GetInstanceAPI().GetDefaultPresentQueue(), &mCommandBuffer, 1, nullptr, 0, nullptr, nullptr, 0, mFence);
	}

	void TextureResource::OnShutdown() noexcept
	{
	}

	void TextureResource::CreateInternalResources()
	{
		CommandPoolDesc poolDesc = { CommandPoolType::Graphics };
		mCommandPool = mGraphicsDevice->CreateCommandPool(poolDesc);

		CommandBufferDesc bufferDesc = { mCommandPool };
		mCommandBuffer = mGraphicsDevice->CreateCommandBuffer(bufferDesc);

		FenceDesc fenceDesc = { true };
		mFence = mGraphicsDevice->CreateSyncFence(fenceDesc);
	}
}

