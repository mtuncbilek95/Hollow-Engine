#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Texture/TextureImageDesc.h>

namespace Hollow
{
	class GraphicsDevice;
	class TextureImage : public DeviceObject
	{
	public:
		TextureImage(const TextureImageDesc& desc, WeakPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice),
			mImageSize(desc.ImageSize), mMipLevels(desc.MipLevels), mArrayLayers(desc.ArrayLayers), 
			mImageFormat(desc.ImageFormat), mUsageFlags(desc.UsageFlags), mImageType(desc.ImageType), 
			mSamples(desc.Samples), mCreateFlags(desc.CreateFlags) {}
		virtual ~TextureImage() override = default;

		const Vec3u& GetImageSize() const { return mImageSize; }
		u32 GetMipLevels() const { return mMipLevels; }
		u32 GetArrayLayers() const { return mArrayLayers; }
		TextureFormat GetImageFormat() const { return mImageFormat; }
		TextureUsageFlags GetUsageFlags() const { return mUsageFlags; }
		TextureType GetImageType() const { return mImageType; }
		SampleCount GetSamples() const { return mSamples; }
		TextureCreateFlags GetCreateFlags() const { return mCreateFlags; }

	private:
		Vec3u mImageSize;
		u32 mMipLevels;
		u32 mArrayLayers;
		TextureFormat mImageFormat;
		TextureUsageFlags mUsageFlags;
		TextureType mImageType;
		SampleCount mSamples;
		TextureCreateFlags mCreateFlags;
	};
}
