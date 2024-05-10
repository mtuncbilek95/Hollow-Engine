#include "VulkanTextureView.h"

#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include <Runtime/Vulkan/Texture/VulkanTexture.h>


namespace Hollow
{
	VulkanTextureView::VulkanTextureView(const TextureViewDesc& desc, VulkanDevice* pDevice) : TextureView(desc, pDevice), mVkDevice(pDevice->GetVkDevice()),
		mVkTextureView(VK_NULL_HANDLE)
	{
		// Create image view
		VkImageViewCreateInfo imageViewInfo = {};
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image = static_cast<VulkanTexture*>(desc.pTexture)->GetVkTexture();
		imageViewInfo.viewType = VulkanTextureUtils::GetTextureViewType(desc.pTexture->GetTextureType());
		imageViewInfo.format = VulkanTextureUtils::GetVkTextureFormat(desc.pTexture->GetImageFormat());
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.subresourceRange.aspectMask = VulkanTextureUtils::GetVkTextureAspectFlags(desc.AspectFlags);
		imageViewInfo.subresourceRange.baseMipLevel = desc.MipLevel;
		imageViewInfo.subresourceRange.levelCount = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = desc.ArrayLayer;
		imageViewInfo.subresourceRange.layerCount = 1;

		CORE_ASSERT(vkCreateImageView(mVkDevice, &imageViewInfo, nullptr, &mVkTextureView) == VK_SUCCESS, "VulkanTextureView", "Failed to create VulkanTextureView.");
	}

	VulkanTextureView::VulkanTextureView(const TextureViewDesc& desc, VkImageView imageView, VulkanDevice* pDevice) : TextureView(desc, pDevice),
		mVkTextureView(imageView), mVkDevice(pDevice->GetVkDevice())
	{
	}

	void VulkanTextureView::OnShutdown() noexcept
	{
		if (mVkTextureView != VK_NULL_HANDLE)
			vkDestroyImageView(mVkDevice, mVkTextureView, nullptr);

		mVkTextureView = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;
	}
}