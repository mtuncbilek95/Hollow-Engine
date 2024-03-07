#include "VulkanTexture.h"

namespace Hollow
{
	VulkanTexture::VulkanTexture(const TextureDesc& desc, VulkanDevice* pDevice) : Texture(desc, pDevice), mVkDevice(pDevice->GetVkDevice()),
		mVkTexture(VK_NULL_HANDLE)
	{
	}

	VulkanTexture::VulkanTexture(const TextureDesc& desc, VkImage image, VulkanDevice* pDevice) : Texture(desc, pDevice), mVkTexture(image),
		mVkDevice(pDevice->GetVkDevice())
	{
	}

	void VulkanTexture::OnShutdown() noexcept
	{
		if (mVkTexture != VK_NULL_HANDLE)
			vkDestroyImage(mVkDevice, mVkTexture, nullptr);

		mVkTexture = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;
	}
}