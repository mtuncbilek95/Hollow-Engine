#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Texture/TextureImage.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class ENGINE_API VDevice;
	class ENGINE_API VTextureImage : public TextureImage
	{
	public:
		VTextureImage(const TextureImageDesc& desc, SharedPtr<VDevice> pDevice);
		VTextureImage(const TextureImageDesc& desc, VkImage texture, SharedPtr<VDevice> pDevice);
		~VTextureImage() override = default;

		VkImage GetVkTexture() const { return mTexture; }
		VkDevice GetVkDevice() const { return mDevice; }

		u64 GetVkOffset() const { return mOffset; }
		u64 GetVkAlignedOffset() const { return mAlignedOffset; }

	private:
		VkImage mTexture;
		VkDevice mDevice;

		u64 mOffset;
		u64 mAlignedOffset;
	};
}
