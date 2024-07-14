#include "VTextureImage.h"
#include "VTextureUtils.h"

#include <Engine/Vulkan/Device/VDevice.h>
#include <Engine/Vulkan/Memory/VMemory.h>

namespace Hollow
{
	VTextureImage::VTextureImage(const TextureImageDesc& desc, SharedPtr<VDevice> pDevice) : TextureImage(desc, pDevice), 
		mTexture(VK_NULL_HANDLE), mDevice(pDevice->GetVkDevice()), mOffset(0), mAlignedOffset(0)
	{
		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = Utils::GetVkImageType(desc.ImageType);
		imageInfo.extent.width = desc.ImageSize.x;
		imageInfo.extent.height = desc.ImageSize.y;
		imageInfo.extent.depth = desc.ImageSize.z;
		imageInfo.mipLevels = desc.MipLevels;
		imageInfo.arrayLayers = desc.ArrayLayers;
		imageInfo.format = Utils::GetVkFormat(desc.ImageFormat);
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = Utils::GetVkImageUsageFlags(desc.UsageFlags);
		imageInfo.samples = Utils::GetVkSampleCountFlags(desc.Samples);
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = Utils::GetVkImageCreateFlags(desc.CreateFlags);

		CORE_ASSERT(vkCreateImage(mDevice, &imageInfo, nullptr, &mTexture) == VK_SUCCESS, "VTextureImage", "Failed to create image!");

		VkMemoryRequirements info = {};
		vkGetImageMemoryRequirements(mDevice, mTexture, &info);

		u64 memoryOffset = desc.pRequestMemory->Allocate(info.size + info.alignment);
		u64 alignedOffset = memoryOffset + (memoryOffset % info.alignment == 0 ? 0 : (info.alignment - (memoryOffset % info.alignment)));

		auto memPtr = desc.pRequestMemory->GetSharedPtrAs<VMemory>();
		CORE_ASSERT(vkBindImageMemory(mDevice, mTexture, memPtr->GetVkDeviceMemory(), alignedOffset) == VK_SUCCESS, "VTextureImage", "Failed to bind image memory!");

		mOffset = memoryOffset;
		mAlignedOffset = alignedOffset;
	}

	VTextureImage::VTextureImage(const TextureImageDesc& desc, VkImage texture, SharedPtr<VDevice> pDevice) : TextureImage(desc, pDevice), 
		mTexture(texture), mDevice(pDevice->GetVkDevice()), mOffset(0), mAlignedOffset(0)
	{
	}
}
