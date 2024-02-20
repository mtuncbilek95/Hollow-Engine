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
			mMipLevels(desc.MipLevels), mFormat(desc.Format), mUsages(desc.Usages), mType(desc.Type), 
			mMemory(desc.pMemory), mCPUAccess(desc.CPUAccess), mSampleCount(desc.SampleCount)
		{}
		virtual ~Texture() override = default;

		const Vector3u& GetImageSize() const { return mImageSize; }
		const byte& GetArraySize() const { return mArraySize; }
		const byte& GetMipLevels() const { return mMipLevels; }
		TextureFormat GetFormat() const { return mFormat; }
		const Array<TextureUsage>& GetUsage() const { return mUsages; }
		BufferCPUAccess GetCPUAccess() const { return mCPUAccess; }
		TextureType GetType() const { return mType; }
		TextureSample GetSampleCount() const { return mSampleCount; }

		const SharedPtr<GraphicsMemory>& GetMemory() const { return mMemory; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept override final { return GraphicsDeviceObjectType::Texture; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		Vector3u mImageSize;
		byte mArraySize;
		byte mMipLevels;
		TextureFormat mFormat;
		Array<TextureUsage> mUsages;
		BufferCPUAccess mCPUAccess;
		TextureSample mSampleCount;
		TextureType mType;
		
		SharedPtr<GraphicsMemory> mMemory;
	};
}
