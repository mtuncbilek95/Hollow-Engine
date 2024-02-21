#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanTexture : public Texture
	{
	public:
		VulkanTexture(const TextureDesc& desc, VkImage image, VulkanDevice* pDevice);
		VulkanTexture(const TextureDesc& desc, VulkanDevice* pDevice);
		~VulkanTexture() override = default;

		const VkImage& GetVkImage() const { return mVkImage; }

		uint64 GetVkAlignedSize() const { return mVkAlignedSize; }
		uint64 GetVkOffset() const { return mVkOffset; }

		void OnShutdown() noexcept override;

	private:
		bool mSwapChainImage;
		VkImage mVkImage;
		VkDevice mVkLogicalDevice;

		uint64 mVkAlignedSize;
		uint64 mVkOffset;
	};
}
