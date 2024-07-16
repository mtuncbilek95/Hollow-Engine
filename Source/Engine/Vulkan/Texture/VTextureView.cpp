#include "VTextureView.h"
#include "VTextureUtils.h"

#include <Engine/Vulkan/Device/VDevice.h>
#include <Engine/Vulkan/Texture/VTextureImage.h>

namespace Hollow
{
	VTextureView::VTextureView(const TextureViewDesc& desc, SharedPtr<VDevice> pDevice) : TextureView(desc, pDevice), mDevice(pDevice->GetVkDevice())
	{
		VkImageViewCreateInfo imageViewInfo = {};
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image = desc.pTexture->GetSharedPtrAs<VTextureImage>()->GetVkTexture();
		imageViewInfo.viewType = VkUtils::GetVkTextureViewType(desc.ViewType);
		imageViewInfo.format = VkUtils::GetVkFormat(desc.pTexture->GetImageFormat());
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.subresourceRange.aspectMask = VkUtils::GetVkTextureAspectFlags(desc.AspectFlags);
		imageViewInfo.subresourceRange.baseMipLevel = desc.MipLevel;
		imageViewInfo.subresourceRange.levelCount = desc.MipCount;
		imageViewInfo.subresourceRange.baseArrayLayer = desc.ArrayLayer;
		imageViewInfo.subresourceRange.layerCount = desc.ArrayCount;

		CORE_ASSERT(vkCreateImageView(mDevice, &imageViewInfo, nullptr, &mImageView) == VK_SUCCESS, "VTextureView", "Failed to create image view!");
	}
}
