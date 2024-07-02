#include "VulkanTextureBuffer.h"

#include <Runtime/Vulkan/Texture/VulkanTexture.h>

#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>

namespace Hollow
{
	VulkanTextureBuffer::VulkanTextureBuffer(const TextureBufferDesc& desc, const SharedPtr<VulkanDevice> pDevice) : TextureBuffer(desc, pDevice), 
		mVkTextureView(VK_NULL_HANDLE)
	{
		mVkDevice = pDevice->GetVkDevice();

		auto pTex = std::static_pointer_cast<VulkanTexture>(desc.pTexture);

		// Create image view
		VkImageViewCreateInfo imageViewInfo = {};
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image = pTex->GetVkTexture();
		imageViewInfo.viewType = VulkanTextureUtils::GetTextureViewType(desc.Type);
		imageViewInfo.format = VulkanTextureUtils::GetVkTextureFormat(desc.pTexture->GetImageFormat());
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.subresourceRange.aspectMask = VulkanTextureUtils::GetVkTextureAspectFlags(desc.AspectFlags);
		imageViewInfo.subresourceRange.baseMipLevel = desc.MipLevel;
		imageViewInfo.subresourceRange.levelCount = desc.MipCount;
		imageViewInfo.subresourceRange.baseArrayLayer = desc.ArrayLayer;
		imageViewInfo.subresourceRange.layerCount = desc.ArrayCount;

		CORE_ASSERT(vkCreateImageView(mVkDevice, &imageViewInfo, nullptr, &mVkTextureView) == VK_SUCCESS, "VulkanTextureView", "Failed to create VulkanTextureView.");
	}

	VulkanTextureBuffer::VulkanTextureBuffer(const TextureBufferDesc& desc, VkImageView imageView, SharedPtr<VulkanDevice> pDevice) : TextureBuffer(desc, pDevice), mVkTextureView(imageView)
	{
		mVkDevice = pDevice->GetVkDevice();
	}

	VulkanTextureBuffer::~VulkanTextureBuffer()
	{
		if (mVkTextureView != VK_NULL_HANDLE)
			vkDestroyImageView(mVkDevice, mVkTextureView, nullptr);

		mVkTextureView = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;
	}
}
