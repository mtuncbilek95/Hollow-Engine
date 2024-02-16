#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Texture/TextureDesc.h>

namespace Hollow
{
	class RUNTIME_API Texture : public GraphicsDeviceObject
	{
	public:
		Texture(const TextureDesc& desc) : mImageSize(desc.ImageSize), mArraySize(desc.ArraySize), 
			mMipLevels(desc.MipLevels), mFormat(desc.Format), mUsage(desc.Usage), mType(desc.Type), 
			mData(desc.Data) 
		{}
		virtual ~Texture() override = default;

		Vector2u GetImageSize() const { return mImageSize; }
		byte GetArraySize() const { return mArraySize; }
		byte GetMipLevels() const { return mMipLevels; }
		TextureFormat GetFormat() const { return mFormat; }
		TextureUsage GetUsage() const { return mUsage; }
		TextureType GetType() const { return mType; }
		uint64 GetData() const { return mData; }

	private:
		Vector2u mImageSize;
		byte mArraySize;
		byte mMipLevels;
		TextureFormat mFormat;
		TextureUsage mUsage;
		TextureType mType;
		uint64 mData;
	};
}
