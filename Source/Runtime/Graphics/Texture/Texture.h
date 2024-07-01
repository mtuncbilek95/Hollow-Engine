#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Texture/TextureDesc.h>

namespace Hollow
{
	/**
	 * @class Texture
	 * @brief Represents the definition of the texture object.
	 */
	class RUNTIME_API Texture : public GraphicsDeviceObject
	{
	public:
		Texture(const TextureDesc& desc, SharedPtr<GraphicsDevice> device) : GraphicsDeviceObject(device), mImageSize(desc.ImageSize),
			mArraySize(desc.ArraySize), mMipLevels(desc.MipLevels), mImageFormat(desc.ImageFormat), mUsage(desc.Usage),
			mSampleCount(desc.SampleCount), mType(desc.Type), mFlags(desc.Flags), mMemory(desc.pMemory)
		{}
		~Texture() override = default;

		FORCEINLINE Vec3u GetImageSize() const noexcept { return mImageSize; }
		FORCEINLINE byte GetArraySize() const noexcept { return mArraySize; }
		FORCEINLINE byte GetMipLevels() const noexcept { return mMipLevels; }
		FORCEINLINE TextureFormat GetImageFormat() const noexcept { return mImageFormat; }
		FORCEINLINE TextureUsage GetUsage() const noexcept { return mUsage; }
		FORCEINLINE TextureSampleCount GetSampleCount() const noexcept { return mSampleCount; }
		FORCEINLINE TextureType GetType() const noexcept { return mType; }

		SharedPtr<GraphicsMemory> GetMemory() const noexcept { return mMemory; }

		FORCEINLINE virtual GraphicsDeviceObjectType GetObjectType() const noexcept override final { return GraphicsDeviceObjectType::Texture; }

	private:
		Vec3u mImageSize;
		byte mArraySize;
		byte mMipLevels;
		TextureFormat mImageFormat;
		TextureUsage mUsage;
		TextureSampleCount mSampleCount;
		TextureType mType;
		TextureCreateFlags mFlags;

		SharedPtr<GraphicsMemory> mMemory;
	};
}
