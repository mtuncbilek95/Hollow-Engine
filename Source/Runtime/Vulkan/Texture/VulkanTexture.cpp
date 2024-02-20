#include "VulkanTexture.h"

#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
namespace Hollow
{
	VulkanTexture::VulkanTexture(const TextureDesc& desc, VulkanDevice* pDevice) : Texture(desc)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();

		VkImageCreateInfo imageInfo = {};

		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VulkanTextureUtils::GetVkImageType(desc.Type);
		imageInfo.extent.width = desc.ImageSize.x;
		imageInfo.extent.height = desc.ImageSize.y;
		imageInfo.extent.depth = desc.ImageSize.z;
		imageInfo.mipLevels = desc.MipLevels;
		imageInfo.arrayLayers = desc.ArraySize;
		imageInfo.format = VulkanTextureUtils::GetVkFormat(desc.Format);
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VulkanTextureUtils::GetVkImageUsageFlags(desc.Usages);
		imageInfo.samples = VulkanTextureUtils::GetVkSampleCount(desc.SampleCount);
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		DEV_ASSERT(vkCreateImage(mVkLogicalDevice, &imageInfo, nullptr, &mVkImage) == VK_SUCCESS, "VulkanTexture", 
			"Failed to create image!");

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(mVkLogicalDevice, mVkImage, &memRequirements);

		DEV_LOG(HE_WARNING, "MemorySize: %zu", memRequirements.size);
		DEV_LOG(HE_WARNING, "MemoryAlignment: %zu", memRequirements.alignment);
		DEV_LOG(HE_WARNING, "MemoryTypeBits: %d", memRequirements.memoryTypeBits);
	}

	void VulkanTexture::OnShutdown() noexcept
	{
		vkDestroyImage(mVkLogicalDevice, mVkImage, nullptr);
	}
}
