#include "VulkanTexture.h"

#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include <Runtime/Vulkan/Memory/VulkanMemory.h>

namespace Hollow
{
	VulkanTexture::VulkanTexture(const TextureDesc& desc, VkImage image, VulkanDevice* pDevice) : Texture(desc, pDevice), mVkImage(image),
		mSwapChainImage(true), mVkAlignedSize(0), mVkOffset(0)
	{
	}

	VulkanTexture::VulkanTexture(const TextureDesc& desc, VulkanDevice* pDevice) : Texture(desc, pDevice), mSwapChainImage(false),
		mVkAlignedSize(0), mVkOffset(0)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();

		VkImageCreateInfo imageInfo = {};

		VulkanMemory* pMemory = reinterpret_cast<VulkanMemory*>(desc.pMemory.get());

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

		uint64 memoryOffset = pMemory->Allocate(memRequirements.size + memRequirements.alignment);
		uint64 memoryAlignedOffset = memoryOffset + (memoryOffset == 0 ? 0 : (memRequirements.alignment - (memoryOffset % memRequirements.alignment)));

		DEV_ASSERT(vkBindImageMemory(mVkLogicalDevice, mVkImage, pMemory->GetVkDeviceMemory(), memoryAlignedOffset) == VK_SUCCESS, "VulkanTexture",
			"Failed to bind image memory!");

		mVkAlignedSize = memoryAlignedOffset;
		mVkOffset = memoryOffset;
	}

	void VulkanTexture::OnShutdown() noexcept
	{
		vkDestroyImage(mVkLogicalDevice, mVkImage, nullptr);
	}
}
