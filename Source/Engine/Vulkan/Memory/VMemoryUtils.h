#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Memory/GraphicsMemoryType.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	namespace Utils
	{
		static VkMemoryPropertyFlags GetVkMemoryPropertyFlags(GraphicsMemoryType memoryType)
		{
			VkMemoryPropertyFlags flags = 0;

			if (memoryType & GraphicsMemoryType::DeviceLocal)
				flags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			if (memoryType & GraphicsMemoryType::HostVisible)
				flags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			if (memoryType & GraphicsMemoryType::HostCoherent)
				flags |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			if (memoryType & GraphicsMemoryType::HostCached)
				flags |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;

			return flags;
		}
	}
}
