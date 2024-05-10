#include "VulkanBuffer.h"

#include <Runtime/Vulkan/Buffer/VulkanBufferUtils.h>
#include <Runtime/Vulkan/Common/VulkanCommonUtils.h>

#include <Runtime/Vulkan/Memory/VulkanMemory.h>

namespace Hollow
{
	VulkanBuffer::VulkanBuffer(const GraphicsBufferDesc& desc, VulkanDevice* pDevice) : GraphicsBuffer(desc, pDevice), mVkBuffer(VK_NULL_HANDLE),
		mVkDevice(pDevice->GetVkDevice()), mOffset(0), mAlignedOffset(0)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = GetTotalSizeInBytes();
		bufferInfo.usage = VulkanBufferUtils::GetVkBufferUsage(desc.Usage);
		bufferInfo.sharingMode = VulkanCommonUtils::GetVkSharingMode(desc.ShareMode);

		CORE_ASSERT(vkCreateBuffer(mVkDevice, &bufferInfo, nullptr, &mVkBuffer) == VK_SUCCESS, "VulkanBuffer", "Failed to create buffer");

		VkMemoryRequirements info = {};
		vkGetBufferMemoryRequirements(mVkDevice, mVkBuffer, &info);

		uint64 memoryOffset = desc.pMemory->AllocateSubMemory(info.alignment + info.size);
		uint64 alignedOffset = memoryOffset + (memoryOffset % info.alignment == 0 ? 0 : (info.alignment - (memoryOffset % info.alignment)));

		CORE_ASSERT(vkBindBufferMemory(mVkDevice, mVkBuffer, reinterpret_cast<VulkanMemory*>(desc.pMemory.get())->GetVkDeviceMemory(), alignedOffset) == VK_SUCCESS,
			"VulkanBuffer", "Failed to bind buffer memory");

		mAlignedOffset = alignedOffset;
		mOffset = memoryOffset;
	}

	void VulkanBuffer::OnShutdown() noexcept
	{
		if(mVkBuffer != VK_NULL_HANDLE)
			vkDestroyBuffer(mVkDevice, mVkBuffer, nullptr);

		mVkBuffer = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;

		CORE_LOG(HE_INFO, "VulkanBuffer", "Buffer has been destroyed");
	}
}