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
		VulkanTexture(const TextureDesc& desc, VulkanDevice* pDevice);
		VulkanTexture(const TextureDesc& desc, VkImage image, VulkanDevice* pDevice);
		~VulkanTexture() override = default;

		VkImage GetVkTexture() const { return mVkTexture; }
		VkDevice GetVkDevice() const { return mVkDevice; }

		void OnShutdown() noexcept override;

	private:
		VkImage mVkTexture;
		VkDevice mVkDevice;
	};
}