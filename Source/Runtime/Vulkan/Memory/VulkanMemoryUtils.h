#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryType.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryAccessFlags.h>

#include <vulkan.h>
namespace Hollow
{
	class RUNTIME_API VulkanMemoryUtils
	{
	public:
		static VkAccessFlags GetVkAccessFlags(GraphicsMemoryAccessFlags flags)
		{
			switch (flags)
			{
			case GraphicsMemoryAccessFlags::None:								return VK_ACCESS_NONE;
			case GraphicsMemoryAccessFlags::IndexRead: 							return VK_ACCESS_INDEX_READ_BIT;
			case GraphicsMemoryAccessFlags::VertexAttributeRead: 				return VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
			case GraphicsMemoryAccessFlags::ConstantRead: 						return VK_ACCESS_UNIFORM_READ_BIT;
			case GraphicsMemoryAccessFlags::InputAttachmentRead: 				return VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
			case GraphicsMemoryAccessFlags::ShaderRead: 						return VK_ACCESS_SHADER_READ_BIT;
			case GraphicsMemoryAccessFlags::ShaderWrite: 						return VK_ACCESS_SHADER_WRITE_BIT;
			case GraphicsMemoryAccessFlags::ColorAttachmentRead: 				return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
			case GraphicsMemoryAccessFlags::ColorAttachmentWrite: 				return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			case GraphicsMemoryAccessFlags::DepthStencilAttachmentRead: 		return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			case GraphicsMemoryAccessFlags::DepthStencilAttachmentWrite: 		return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			case GraphicsMemoryAccessFlags::TransferRead: 						return VK_ACCESS_TRANSFER_READ_BIT;
			case GraphicsMemoryAccessFlags::TransferWrite: 						return VK_ACCESS_TRANSFER_WRITE_BIT;
			case GraphicsMemoryAccessFlags::HostRead: 							return VK_ACCESS_HOST_READ_BIT;
			case GraphicsMemoryAccessFlags::HostWrite: 							return VK_ACCESS_HOST_WRITE_BIT;
			case GraphicsMemoryAccessFlags::MemoryRead: 						return VK_ACCESS_MEMORY_READ_BIT;
			case GraphicsMemoryAccessFlags::MemoryWrite: 						return VK_ACCESS_MEMORY_WRITE_BIT;
			default:															return VK_ACCESS_NONE;
			}
		}

		static VkMemoryPropertyFlags GetVkMemoryType(GraphicsMemoryType type)
		{
			switch (type)
			{
			case GraphicsMemoryType::CPU:
				return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			case GraphicsMemoryType::GPU:
				return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			default:
				return VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM;
			}
		}
	};
}
