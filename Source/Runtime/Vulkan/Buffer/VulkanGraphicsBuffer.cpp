#include "VulkanGraphicsBuffer.h"

#include <Runtime/Vulkan/Buffer/VulkanBufferUtils.h>
#include <Runtime/Vulkan/Memory/VulkanMemory.h>

namespace Hollow
{
	VulkanGraphicsBuffer::VulkanGraphicsBuffer(const GraphicsBufferDesc& desc, VulkanDevice* pDevice) : GraphicsBuffer(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();

		// Create the buffer
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = GetSizeInBytes();
		bufferInfo.usage = VulkanBufferUtils::GetVkBufferUsageFlags(desc.Usage);
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferInfo.flags = VkBufferCreateFlags();
		bufferInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateBuffer(mVkLogicalDevice, &bufferInfo, nullptr, &mVkGraphicsBuffer) == VK_SUCCESS, 
						"VulkanGraphicsBuffer", "Failed to create buffer.");

		// Allocate memory for the buffer
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(mVkLogicalDevice, mVkGraphicsBuffer, &memRequirements);

		uint64 memoryOffset = desc.pMemory->Allocate(memRequirements.alignment + memRequirements.size);
		uint64 alignedMemoryOffset = memoryOffset + (memoryOffset == 0 ? 0 : (memRequirements.alignment - (memoryOffset % memRequirements.alignment)));

		VulkanMemory* pMemory = static_cast<VulkanMemory*>(desc.pMemory.get());
		DEV_ASSERT(vkBindBufferMemory(mVkLogicalDevice, mVkGraphicsBuffer, pMemory->GetVkDeviceMemory(), alignedMemoryOffset) == VK_SUCCESS,
									"VulkanGraphicsBuffer", "Failed to bind buffer memory.");

		mVkMemoryOffset = memoryOffset;
		mVkMemorySize = alignedMemoryOffset;
	}

	void VulkanGraphicsBuffer::OnShutdown() noexcept
	{
		if (mVkGraphicsBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(mVkLogicalDevice, mVkGraphicsBuffer, nullptr);
			mVkGraphicsBuffer = VK_NULL_HANDLE;
		}
	}
}