#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Buffer/GraphicsBufferUsage.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	namespace VkUtils
	{
		static VkBufferUsageFlags GetVKBufferUsage(GraphicsBufferUsage usage)
		{
			VkBufferUsageFlags flags = 0;

			if (usage & GraphicsBufferUsage::Vertex)
				flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			else if (usage & GraphicsBufferUsage::Index)
				flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			else if (usage & GraphicsBufferUsage::Uniform)
				flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			else if (usage & GraphicsBufferUsage::Storage)
				flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

			if (usage & GraphicsBufferUsage::TransferDst)
				flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			else if (usage & GraphicsBufferUsage::TransferSrc)
				flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

			return flags;
		}
	}
}
