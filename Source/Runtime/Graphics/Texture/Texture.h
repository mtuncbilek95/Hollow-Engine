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
			mMemory(desc.pMemory), mCPUAccess(desc.CPUAccess)
		{}
		virtual ~Texture() override = default;

		const Vector2u& GetImageSize() const { return mImageSize; }
		const byte& GetArraySize() const { return mArraySize; }
		const byte& GetMipLevels() const { return mMipLevels; }
		TextureFormat GetFormat() const { return mFormat; }
		TextureUsage GetUsage() const { return mUsage; }
		BufferCPUAccess GetCPUAccess() const { return mCPUAccess; }
		TextureType GetType() const { return mType; }
		const SharedPtr<GraphicsMemory>& GetMemory() const { return mMemory; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept override final { return GraphicsDeviceObjectType::Texture; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		Vector2u mImageSize;
		byte mArraySize;
		byte mMipLevels;
		TextureFormat mFormat;
		TextureUsage mUsage;
		BufferCPUAccess mCPUAccess;
		TextureType mType;
		
		SharedPtr<GraphicsMemory> mMemory;
	};
}
