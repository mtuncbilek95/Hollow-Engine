#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanTexture final : public Texture
	{
	public:
		VulkanTexture(const TextureDesc& desc, const SharedPtr<VulkanDevice> pDevice);
		VulkanTexture(const TextureDesc& desc, VkImage image, SharedPtr<VulkanDevice> pDevice);
		~VulkanTexture() override;

		VkImage GetVkTexture() const { return mVkTexture; }
		VkDevice GetVkDevice() const { return mVkDevice; }

		u64 GetVkOffset() const { return mOffset; }
		u64 GetVkAlignedOffset() const { return mAlignedOffset; }

	private:
		VkImage mVkTexture;
		VkDevice mVkDevice;

		u64 mOffset;
		u64 mAlignedOffset;
	};
}
