#include "VulkanTextureView.h"

#include <Runtime/Vulkan/Texture/VulkanTexture.h>

#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>

namespace Hollow
{
	VulkanTextureView::VulkanTextureView(const TextureViewDesc& desc, VkImageView imageView, VulkanDevice* pDevice) : TextureView(desc, pDevice),
		mVkTextureView(imageView), mSwapchainImageView(true), mVkLogicalDevice(VK_NULL_HANDLE)
	{
	}

	VulkanTextureView::VulkanTextureView(const TextureViewDesc& desc, VulkanDevice* pDevice) : TextureView(desc, pDevice),
		mVkTextureView(VK_NULL_HANDLE), mSwapchainImageView(false)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();

		SharedPtr<VulkanTexture> attachTexture = std::dynamic_pointer_cast<VulkanTexture>(desc.pTexture);

		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = attachTexture->GetVkImage();
		imageViewCreateInfo.viewType = VulkanTextureUtils::GetTextureViewType(desc.pTexture->GetType());
		imageViewCreateInfo.format = VulkanTextureUtils::GetVkTextureFormat(desc.pTexture->GetFormat());
		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.subresourceRange.aspectMask = VulkanTextureUtils::GetVkTextureAspects(desc.Aspects);
		imageViewCreateInfo.subresourceRange.baseArrayLayer = desc.ArraySize;
		imageViewCreateInfo.subresourceRange.baseMipLevel = desc.MipLevels;
		imageViewCreateInfo.subresourceRange.layerCount = desc.pTexture->GetArraySize();
		imageViewCreateInfo.subresourceRange.levelCount = desc.pTexture->GetMipLevels();
		imageViewCreateInfo.flags = VkImageViewCreateFlags();
		imageViewCreateInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateImageView(mVkLogicalDevice, &imageViewCreateInfo, nullptr, &mVkTextureView) == VK_SUCCESS, 
			"VulkanTextureView", "Failed to create image view");
	}

	void VulkanTextureView::OnShutdown() noexcept
	{
		if (mVkTextureView != VK_NULL_HANDLE)
		{
			vkDestroyImageView(mVkLogicalDevice, mVkTextureView, nullptr);
		}
	}
}