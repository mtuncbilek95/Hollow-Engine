#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Texture/TextureViewDesc.h>

namespace Hollow
{
	/**
	 * @class TextureView.
	 * 
	 * @brief We can call this one as ShaderResourceView storage as well. It is 
	 * the graphical representation of the texture.
	 * 
	 */
	class RUNTIME_API TextureView : public GraphicsDeviceObject
	{
	public:
		TextureView(const TextureViewDesc& desc) : mArraySize(desc.ArraySize), mMipLevels(desc.MipLevels), mTexture(desc.pTexture) 
		{}
		virtual ~TextureView() override = default;

		const byte& GetArraySize() const { return mArraySize; }
		const byte& GetMipLevels() const { return mMipLevels; }
		const SharedPtr<Texture>& GetTexture() const { return mTexture; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept override final { return GraphicsDeviceObjectType::TextureView; }

	private:
		byte mArraySize;
		byte mMipLevels;
		SharedPtr<Texture> mTexture;
	};
}
