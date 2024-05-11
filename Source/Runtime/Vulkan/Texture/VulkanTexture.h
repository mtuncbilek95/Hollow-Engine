#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanTexture final : public Texture
	{
	public:
		VulkanTexture(const TextureDesc& desc, const SharedPtr<VulkanDevice> pDevice);
		VulkanTexture(const TextureDesc& desc, VkImage image, SharedPtr<VulkanDevice> pDevice);
		~VulkanTexture() override = default;

		VkImage GetVkTexture() const { return mVkTexture; }
		VkDevice GetVkDevice() const { return mVkDevice; }

		uint64 GetVkOffset() const { return mOffset; }
		uint64 GetVkAlignedOffset() const { return mAlignedOffset; }

		void OnShutdown() noexcept override;

	private:
		VkImage mVkTexture;
		VkDevice mVkDevice;

		uint64 mOffset;
		uint64 mAlignedOffset;
	};
}
