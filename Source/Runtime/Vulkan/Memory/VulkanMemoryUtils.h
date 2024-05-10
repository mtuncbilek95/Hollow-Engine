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
		static VkMemoryPropertyFlagBits GetVkMemoryType(GraphicsMemoryType type)
		{
			switch (type)
			{
			case GraphicsMemoryType::DeviceLocal:
				return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			case GraphicsMemoryType::HostVisible:
				return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			case GraphicsMemoryType::HostCoherent:
				return VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			case GraphicsMemoryType::HostCached:
				return VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
			default:
				return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			}
		}

		static VkAccessFlags GetVkAccessFlags(GraphicsMemoryAccessFlags flags)
		{
            VkAccessFlags flagsOut = VkAccessFlags();

            if (flags & GraphicsMemoryAccessFlags::Unknown)
                return VK_ACCESS_NONE;

            if (flags & GraphicsMemoryAccessFlags::IndexRead)
                flagsOut |= VK_ACCESS_INDEX_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::VertexAttributeRead)
                flagsOut |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::UniformRead)
                flagsOut |= VK_ACCESS_UNIFORM_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::InputAttachmentRead)
                flagsOut |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::ShaderRead)
                flagsOut |= VK_ACCESS_SHADER_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::ShaderWrite)
                flagsOut |= VK_ACCESS_SHADER_WRITE_BIT;
            if (flags & GraphicsMemoryAccessFlags::ColorAttachmentRead)
                flagsOut |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::ColorAttachmentWrite)
                flagsOut |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            if (flags & GraphicsMemoryAccessFlags::DepthStencilAttachmentRead)
                flagsOut |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::DepthStencilAttachmentWrite)
                flagsOut |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            if (flags & GraphicsMemoryAccessFlags::TransferRead)
                flagsOut |= VK_ACCESS_TRANSFER_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::TransferWrite)
                flagsOut |= VK_ACCESS_TRANSFER_WRITE_BIT;
            if (flags & GraphicsMemoryAccessFlags::HostRead)
                flagsOut |= VK_ACCESS_HOST_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::HostWrite)
                flagsOut |= VK_ACCESS_HOST_WRITE_BIT;
            if (flags & GraphicsMemoryAccessFlags::MemoryRead)
                flagsOut |= VK_ACCESS_MEMORY_READ_BIT;
            if (flags & GraphicsMemoryAccessFlags::MemoryWrite)
                flagsOut |= VK_ACCESS_MEMORY_WRITE_BIT;

            return flagsOut;
		}

    public:
        VulkanMemoryUtils() = delete;
        ~VulkanMemoryUtils() = delete;
	};
}