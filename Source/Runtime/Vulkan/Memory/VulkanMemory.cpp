#include "VulkanMemory.h"

#include <Runtime/Vulkan/Buffer/VulkanBuffer.h>
#include <Runtime/Vulkan/Texture/VulkanTexture.h>
#include <Runtime/Vulkan/Memory/VulkanMemoryUtils.h>

namespace Hollow
{
	VulkanMemory::VulkanMemory(const GraphicsMemoryDesc& desc, VulkanDevice* pDevice) : GraphicsMemory(desc, pDevice),
		mVkDevice(pDevice->GetVkDevice()), mVkPhysicalDevice(pDevice->GetVkPhysicalDevice()), mVkDeviceMemory(VK_NULL_HANDLE)
	{
		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = desc.SizeInBytes;
		allocInfo.memoryTypeIndex = FindMemoryType(desc.MemoryType);
		allocInfo.pNext = nullptr;

		CORE_ASSERT(vkAllocateMemory(mVkDevice, &allocInfo, nullptr, &mVkDeviceMemory) == VK_SUCCESS, "VulkanMemory", "Failed to allocate memory");
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

	uint32 VulkanMemory::FindMemoryType(GraphicsMemoryType type)
	{
		VkMemoryPropertyFlagBits properties = VulkanMemoryUtils::GetVkMemoryType(type);

		VkPhysicalDeviceMemoryProperties memProperties = {};
		vkGetPhysicalDeviceMemoryProperties(mVkPhysicalDevice, &memProperties);

		for (uint32 i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if (memProperties.memoryTypes[i].propertyFlags & properties)
				return i;
		}

		CORE_LOG(HE_ERROR, "VulkanMemory", "Failed to find suitable memory type!");
		return 0;
	}
}