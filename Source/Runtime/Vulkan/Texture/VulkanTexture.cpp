#include "VulkanTexture.h"

#include <Runtime/Vulkan/Memory/VulkanMemory.h>
#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>

namespace Hollow
{
	VulkanTexture::VulkanTexture(const TextureDesc& desc, const SharedPtr<VulkanDevice> pDevice) : Texture(desc, pDevice), mVkTexture(VK_NULL_HANDLE), 
		mVkDevice(pDevice->GetVkDevice()), mAlignedOffset(0), mOffset(0)
	{
		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VulkanTextureUtils::GetVkTextureType(desc.Type);
		imageInfo.extent.width = desc.ImageSize.x;
		imageInfo.extent.height = desc.ImageSize.y;
		imageInfo.extent.depth = desc.ImageSize.z;
		imageInfo.mipLevels = desc.MipLevels;
		imageInfo.arrayLayers = desc.ArraySize;
		imageInfo.format = VulkanTextureUtils::GetVkTextureFormat(desc.ImageFormat);
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VulkanTextureUtils::GetVkTextureUsageFlags(desc.Usage);
		imageInfo.samples = VulkanTextureUtils::GetVkSampleCount(desc.SampleCount);
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = 0;

		CORE_ASSERT(vkCreateImage(mVkDevice, &imageInfo, nullptr, &mVkTexture) == VK_SUCCESS, "VulkanTexture", "Failed to create image");

		VkMemoryRequirements info = {};
		vkGetImageMemoryRequirements(mVkDevice, mVkTexture, &info);

		u64 memoryOffset = desc.pMemory->AllocateSubMemory(info.alignment + info.size);
		u64 alignedOffset = memoryOffset + (memoryOffset % info.alignment == 0 ? 0 : (info.alignment - (memoryOffset % info.alignment)));

		auto memPtr = std::static_pointer_cast<VulkanMemory>(desc.pMemory);
		CORE_ASSERT(vkBindImageMemory(mVkDevice, mVkTexture, memPtr->GetVkDeviceMemory(), alignedOffset) == VK_SUCCESS,
			"VulkanTexture", "Failed to bind image memory");

		mOffset = memoryOffset;
		mAlignedOffset = alignedOffset;
	}

	VulkanTexture::VulkanTexture(const TextureDesc& desc, VkImage image, SharedPtr<VulkanDevice> pDevice) : Texture(desc, pDevice), mVkTexture(image),
		mVkDevice(pDevice->GetVkDevice()), mAlignedOffset(0), mOffset(0)
	{
	}

	VulkanTexture::~VulkanTexture()
	{
		if (mVkTexture != VK_NULL_HANDLE)
			vkDestroyImage(mVkDevice, mVkTexture, nullptr);

		mVkTexture = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;
	}
}
