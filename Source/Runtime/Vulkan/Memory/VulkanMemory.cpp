#include "VulkanMemory.h"

#include <Runtime/Vulkan/Memory/VulkanMemoryUtils.h>

namespace Hollow
{
	VulkanMemory::VulkanMemory(const GraphicsMemoryDesc& desc, VulkanDevice* pDevice) : GraphicsMemory(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = desc.SizeInBytes;
		allocInfo.memoryTypeIndex = VulkanMemoryUtils::GetVkMemoryType(desc.Type);
		allocInfo.pNext = nullptr;

		DEV_ASSERT(vkAllocateMemory(mVkLogicalDevice, &allocInfo, nullptr, &mVkDeviceMemory) == VK_SUCCESS, 
			"VulkanMemory", "Failed to allocate memory.");
	}

	void VulkanMemory::OnShutdown() noexcept
	{
		if(mVkDeviceMemory != VK_NULL_HANDLE)
		{
			vkFreeMemory(mVkLogicalDevice, mVkDeviceMemory, nullptr);
			mVkDeviceMemory = VK_NULL_HANDLE;
		}
	}
}
