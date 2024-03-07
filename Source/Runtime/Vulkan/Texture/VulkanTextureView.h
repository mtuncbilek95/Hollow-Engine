#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureView.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanTextureView : public TextureView
	{
	public:
		VulkanTextureView(const TextureViewDesc& desc, VulkanDevice* pDevice);
		VulkanTextureView(const TextureViewDesc& desc, VkImageView imageView, VulkanDevice* pDevice);
		~VulkanTextureView() override = default;

		VkImageView GetVkTextureView() const { return mVkTextureView; }
		VkDevice GetVkDevice() const { return mVkDevice; }

		void OnShutdown() noexcept override;

	private:
		VkImageView mVkTextureView;
		VkDevice mVkDevice;
	};
}