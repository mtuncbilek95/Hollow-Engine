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
		~VulkanTexture() override = default;

		const VkImage& GetVkImage() const { return mVkImage; }

		void OnShutdown() noexcept override;

	private:
		VkImage mVkImage;
		VkDevice mVkLogicalDevice;
	};
}
