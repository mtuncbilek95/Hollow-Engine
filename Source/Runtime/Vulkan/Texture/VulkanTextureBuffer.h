#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureBuffer.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanTextureBuffer final : public TextureBuffer
	{
	public:
		VulkanTextureBuffer(const TextureBufferDesc& desc,const SharedPtr<VulkanDevice> pDevice);
		VulkanTextureBuffer(const TextureBufferDesc& desc, VkImageView imageView, SharedPtr<VulkanDevice> pDevice);
		~VulkanTextureBuffer() override = default;

		VkImageView GetVkTextureView() const { return mVkTextureView; }
		VkDevice GetVkDevice() const { return mVkDevice; }

		void OnShutdown() noexcept override;

	private:
		VkImageView mVkTextureView;
		VkDevice mVkDevice;
	};
}
