#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Buffer/GraphicsBufferUsage.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanBufferUtils
	{
	public:
		static VkBufferUsageFlags GetVkBufferUsageFlags(GraphicsBufferUsage usages)
		{
            VkBufferUsageFlags flags = VkBufferUsageFlags();

            if (usages & GraphicsBufferUsage::VertexBuffer)
                flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            else if (usages & GraphicsBufferUsage::IndexBuffer)
                flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            else if (usages & GraphicsBufferUsage::ConstantBuffer)
                flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            else if (usages & GraphicsBufferUsage::Storage)
                flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

            if (usages & GraphicsBufferUsage::TransferDestination)
                flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            else if (usages & GraphicsBufferUsage::TransferSource)
                flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

            return flags;
		}
	};
}