#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Texture/TextureViewDesc.h>

namespace Hollow
{
	class RUNTIME_API TextureView : public GraphicsDeviceObject
	{
	public:
		TextureView(const TextureViewDesc& desc, GraphicsDevice* pDevice) : mTexture(desc.pTexture), mAspectFlags(desc.AspectFlags), 
			mMipLevel(desc.MipLevel), mArrayLayer(desc.ArrayLayer), GraphicsDeviceObject(pDevice)
		{}
		virtual ~TextureView() override = default;

		Texture* GetTexture() const noexcept { return mTexture; }
		TextureAspectFlags GetAspectFlags() const noexcept { return mAspectFlags; }
		byte GetMipLevel() const noexcept { return mMipLevel; }
		byte GetarrayListLayer() const noexcept { return mArrayLayer; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::Texture; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		Texture* mTexture;
		TextureAspectFlags mAspectFlags;
		byte mMipLevel;
		byte mArrayLayer;
	};
}