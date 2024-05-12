#include "TextureResource.h"

#include <Runtime/Graphics/Texture/TextureUtils.h>

namespace Hollow
{
	TextureResource::TextureResource() : ResourceSubObject(), mMipDataInitialized(false)
	{
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

		mMipData.reserve(desc.ArraySize);
		for (byte i = 0; i < desc.ArraySize; i++)
		{
			mMipData[i].reserve(desc.MipLevels);
		}

		for (byte i = 0; i < desc.ArraySize; i++)
		{
			for (byte j = 0; j < desc.MipLevels; j++)
			{
				if (preAllocData)
				{
					GraphicsBufferDesc bufferDesc = {};
					bufferDesc.pMemory = mHostMemory;
					bufferDesc.ShareMode = ShareMode::Exclusive;
					bufferDesc.SubResourceCount = 1;
					bufferDesc.SubSizeInBytes = mTexture->GetImageSize().x * mTexture->GetImageSize().y * TextureUtils::GetTextureFormatSize(mTexture->GetImageFormat());
					bufferDesc.Usage = GraphicsBufferUsage::TransferSource;

					mMipData[i][j].MipStageBuffer = mGraphicsDevice->CreateGraphicsBuffer(bufferDesc);
				}

				if (preAllocMips)
				{
					mMipData[i][j].MipTextureBuffer = CreateResourceBuffer(TextureAspectFlags::ColorAspect, j, i);
				}
			}
		}
	}

	void TextureResource::UpdateTextureData(const MemoryBuffer& data, const Vector3u& offset, const TextureMemoryLayout inputMemoryLayout, const GraphicsMemoryAccessFlags inputAccessFlags, const PipelineStageFlags inputPipelineFlags, const GraphicsQueueType inputQueueType, const byte mipLevel, const byte arrayLevel)
	{
		BufferDataUpdateDesc textureDataUpdateDesc = {};
		textureDataUpdateDesc.Memory = data;
		textureDataUpdateDesc.OffsetInBytes = 0;
		mGraphicsDevice->UpdateBufferData(mMipData[arrayLevel][mipLevel].MipStageBuffer, textureDataUpdateDesc);

		// First, make sure that texture is ready to be written.
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
		textureCopyDesc.TextureSize = mDesc.ImageSize;
		mCommandBuffer->CopyBufferToTexture(mMipData[arrayLevel][mipLevel].MipStageBuffer, mTexture, textureCopyDesc);

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
