#include "VulkanTexture.h"

#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include <Runtime/Vulkan/Common/VulkanCommonUtils.h>

#include <Runtime/Vulkan/Memory/VulkanMemory.h>

namespace Hollow
{
	VulkanTexture::VulkanTexture(const TextureDesc& desc, VulkanDevice* pDevice) : Texture(desc, pDevice), mVkDevice(pDevice->GetVkDevice()),
		mVkTexture(VK_NULL_HANDLE), mOffset(0), mAlignedOffset(0)
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

		uint64 memoryOffset = desc.pMemory->AllocateSubMemory(info.alignment + info.size);
		uint64 alignedOffset = memoryOffset + (memoryOffset % info.alignment == 0 ? 0 : (info.alignment - (memoryOffset % info.alignment)));

		CORE_ASSERT(vkBindImageMemory(mVkDevice, mVkTexture, reinterpret_cast<VulkanMemory*>(desc.pMemory.get())->GetVkDeviceMemory(), alignedOffset) == VK_SUCCESS,
			"VulkanTexture", "Failed to bind image memory");

		mOffset = memoryOffset;
		mAlignedOffset = alignedOffset;
	}

	VulkanTexture::VulkanTexture(const TextureDesc& desc, VkImage image, VulkanDevice* pDevice) : Texture(desc, pDevice), mVkTexture(image),
		mVkDevice(pDevice->GetVkDevice()), mOffset(0), mAlignedOffset(0)
	{
	}

	void VulkanTexture::OnShutdown() noexcept
	{
		if (mVkTexture != VK_NULL_HANDLE)
			vkDestroyImage(mVkDevice, mVkTexture, nullptr);

		mVkTexture = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;

		CORE_LOG(HE_INFO, "VulkanTexture", "Texture has been destroyed");
	}
}