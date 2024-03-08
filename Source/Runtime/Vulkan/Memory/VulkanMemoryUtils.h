#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryType.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanMemoryUtils
	{
	public:
		static VkMemoryPropertyFlags GetVkMemoryType(GraphicsMemoryType type)
		{
			VkMemoryPropertyFlags flags = 0;

			if (type & GraphicsMemoryType::Local)
				flags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			if (type & GraphicsMemoryType::Host)
				flags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

			return flags;
		}
	};
}