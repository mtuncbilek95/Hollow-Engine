#include "VulkanMemory.h"

#include <Runtime/Vulkan/Buffer/VulkanBuffer.h>
#include <Runtime/Vulkan/Memory/VulkanMemoryUtils.h>

namespace Hollow
{
	VulkanMemory::VulkanMemory(const GraphicsMemoryDesc& desc, VulkanDevice* pDevice) : GraphicsMemory(desc, pDevice),
		mVkDevice(pDevice->GetVkDevice()), mVkPhysicalDevice(pDevice->GetVkPhysicalDevice()), mVkDeviceMemory(VK_NULL_HANDLE)
	{
		VkMemoryRequirements memRequirements = {};
		vkGetBufferMemoryRequirements(mVkDevice, static_cast<VulkanBuffer*>(desc.pOwnerBuffer)->GetVkBuffer(), &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, desc.MemoryType);

		DEV_ASSERT(vkAllocateMemory(mVkDevice, &allocInfo, nullptr, &mVkDeviceMemory) == VK_SUCCESS, "VulkanMemory", "Failed to allocate buffer memory!");
		DEV_ASSERT(vkBindBufferMemory(mVkDevice, static_cast<VulkanBuffer*>(desc.pOwnerBuffer)->GetVkBuffer(), mVkDeviceMemory, 0) == VK_SUCCESS, "VulkanMemory", 
			"Failed to bind buffer memory!");
	}

	void VulkanMemory::OnShutdown() noexcept
	{
		if (mVkDeviceMemory != VK_NULL_HANDLE)
			vkFreeMemory(mVkDevice, mVkDeviceMemory, nullptr);

		mVkDeviceMemory = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;
		mVkPhysicalDevice = VK_NULL_HANDLE;

		CORE_LOG(HE_INFO, "VulkanMemory", "Memory has been destroyed");
	}

	uint32 VulkanMemory::FindMemoryType(uint32 typeFilter, GraphicsMemoryType type)
	{
		VkMemoryPropertyFlags properties = VulkanMemoryUtils::GetVkMemoryType(type);

		VkPhysicalDeviceMemoryProperties memProperties = {};
		vkGetPhysicalDeviceMemoryProperties(mVkPhysicalDevice, &memProperties);

		for (uint32 i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
		}

		DEV_LOG(HE_ERROR, "VulkanBuffer", "Failed to find suitable memory type!");
		return 0;
	}
}