#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Texture/TextureBufferDesc.h>

namespace Hollow
{
	/**
	 * @class TextureBuffer
	 * @brief Represents the data of the texture in the GPU.
	 */
	class RUNTIME_API TextureBuffer : public GraphicsDeviceObject
	{
	public:
		TextureBuffer(const TextureBufferDesc& desc, SharedPtr<GraphicsDevice> device) : GraphicsDeviceObject(device), 
			mTexture(desc.pTexture), mAspectFlags(desc.AspectFlags), mMipLevel(desc.MipLevel), mArrayLayer(desc.ArrayLayer) 
		{}
		virtual ~TextureBuffer() override = default;

		SharedPtr<Texture> GetTexture() const noexcept { return mTexture; }
		FORCEINLINE TextureAspectFlags GetAspectFlags() const noexcept { return mAspectFlags; }
		FORCEINLINE byte GetMipLevel() const noexcept { return mMipLevel; }
		FORCEINLINE byte GetArrayLayer() const noexcept { return mArrayLayer; }

		FORCEINLINE virtual GraphicsDeviceObjectType GetObjectType() const noexcept override final { return GraphicsDeviceObjectType::TextureBuffer; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		SharedPtr<Texture> mTexture;
		TextureAspectFlags mAspectFlags;
		byte mMipLevel;
		byte mArrayLayer;
	};
}
