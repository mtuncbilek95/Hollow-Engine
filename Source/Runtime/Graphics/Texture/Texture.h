#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Texture/TextureDesc.h>

namespace Hollow
{
	class RUNTIME_API Texture : public GraphicsDeviceObject
	{
	public:
		Texture(const TextureDesc& desc, GraphicsDevice* pDevice) : mImageSize(desc.ImageSize), mArraySize(desc.ArraySize),
			mMipLevels(desc.MipLevels), mImageFormat(desc.ImageFormat), mUsage(desc.Usage), mSampleCount(desc.SampleCount), 
			mType(desc.Type), GraphicsDeviceObject(pDevice) 
		{}
		virtual ~Texture() override = default;

		const Vector3u& GetImageSize() const noexcept { return mImageSize; }
		byte GetArraySize() const noexcept { return mArraySize; }
		byte GetMipLevels() const noexcept { return mMipLevels; }
		TextureFormat GetImageFormat() const noexcept { return mImageFormat; }
		TextureUsage GetUsage() const noexcept { return mUsage; }
		TextureSampleCount GetSampleCount() const noexcept { return mSampleCount; }
		TextureType GetTextureType() const noexcept { return mType; }

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
	};
}