#include "VMemory.h"
#include "VMemoryUtils.h"

#include <Engine/Vulkan/Device/VDevice.h>

namespace Hollow
{
	VMemory::VMemory(const GraphicsMemoryDesc& desc, SharedPtr<VDevice> pDevice) : GraphicsMemory(desc, pDevice),
		mVkAdapter(pDevice->GetVkAdapter()), mDevice(pDevice->GetVkDevice())
	{
		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.allocationSize = desc.MemorySize;
		memoryAllocateInfo.memoryTypeIndex = FindMemoryType(desc.MemoryType);

		CORE_ASSERT(vkAllocateMemory(mDevice, &memoryAllocateInfo, nullptr, &mVkDeviceMemory) == VK_SUCCESS, "VMemory", "Failed to allocate memory");
	}

	u32 VMemory::FindMemoryType(GraphicsMemoryType memoryType)
	{
		VkMemoryPropertyFlags properties = VkUtils::GetVkMemoryPropertyFlags(memoryType);

		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(mVkAdapter, &memoryProperties);

		for (u32 i = 0; i < memoryProperties.memoryTypeCount; i++)
			if ((memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
				return i;

		CORE_ASSERT(false, "VMemory", "Failed to find suitable memory type");
		return u32_max;
	}
}