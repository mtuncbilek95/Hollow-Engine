#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Texture/TextureDesc.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>

namespace Hollow
{
	class RUNTIME_API Texture : public GraphicsDeviceObject
	{
	public:
		Texture(const TextureDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), mImageSize(desc.ImageSize), mArraySize(desc.ArraySize),
			mMipLevels(desc.MipLevels), mImageFormat(desc.ImageFormat), mUsage(desc.Usage), mSampleCount(desc.SampleCount), 
			mType(desc.Type), mMemory(desc.pMemory)
		{}
		virtual ~Texture() override = default;

		const Vector3u& GetImagesize() const noexcept { return mImageSize; }
		byte GetArraySize() const noexcept { return mArraySize; }
		byte GetMipLevels() const noexcept { return mMipLevels; }
		TextureFormat GetImageFormat() const noexcept { return mImageFormat; }
		TextureUsage GetUsage() const noexcept { return mUsage; }
		TextureSampleCount GetSampleCount() const noexcept { return mSampleCount; }
		TextureType GetTextureType() const noexcept { return mType; }

		GraphicsMemory* GetMemory() const noexcept { return mMemory.get(); }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::Texture; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		Vector3u mImageSize;
		byte mArraySize;
		byte mMipLevels;
		TextureFormat mImageFormat;
		TextureUsage mUsage;
		TextureSampleCount mSampleCount;
		TextureType mType;

		sharedPtr<GraphicsMemory> mMemory;
	};
}