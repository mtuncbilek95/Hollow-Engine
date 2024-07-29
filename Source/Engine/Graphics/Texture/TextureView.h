#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Texture/TextureViewDesc.h>

namespace Hollow
{
	class GraphicsDevice;
	class TextureView : public DeviceObject
	{
	public:
		TextureView(const TextureViewDesc& desc, WeakPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice),
			mMipLevel(desc.MipLevel), mMipCount(desc.MipCount), mArrayLayer(desc.ArrayLayer), mArrayCount(desc.ArrayCount), 
			mAspectFlags(desc.AspectFlags), mViewType(desc.ViewType), mTexture(desc.pTexture) {}
		virtual ~TextureView() override = default;

		u8 GetMipLevel() const { return mMipLevel; }
		u8 GetMipCount() const { return mMipCount; }
		u8 GetArrayLayer() const { return mArrayLayer; }
		u8 GetArrayCount() const { return mArrayCount; }
		TextureAspectFlags GetAspectFlags() const { return mAspectFlags; }
		TextureViewType GetViewType() const { return mViewType; }
		
		WeakPtr<TextureImage> GetTexture() const { return mTexture; }

	private:
		u8 mMipLevel;
		u8 mMipCount;
		u8 mArrayLayer;
		u8 mArrayCount;
		TextureAspectFlags mAspectFlags;
		TextureViewType mViewType;
		WeakPtr<TextureImage> mTexture;
	};
}
