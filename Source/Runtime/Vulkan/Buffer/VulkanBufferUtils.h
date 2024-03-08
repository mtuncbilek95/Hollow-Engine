#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Buffer/GraphicsBufferUsage.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanBufferUtils
	{
	public:
		static VkBufferUsageFlags GetVkBufferUsage(GraphicsBufferUsage usage)
		{
			VkBufferUsageFlags flags = 0;

			if(usage & GraphicsBufferUsage::Vertex)
				flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			else if(usage & GraphicsBufferUsage::Index)
				flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			else if(usage & GraphicsBufferUsage::Uniform)
				flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			else if(usage & GraphicsBufferUsage::Storage)
				flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
			
			if(usage & GraphicsBufferUsage::TransferDestination)
				flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			else if(usage & GraphicsBufferUsage::TransferSource)
				flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

			return flags;
		}

	public:
		VulkanBufferUtils() = delete;
		~VulkanBufferUtils() = delete;
	};
}