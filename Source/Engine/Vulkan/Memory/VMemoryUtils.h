#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Memory/GraphicsMemoryType.h>
#include <Engine/Graphics/Memory/GraphicsMemoryAccessFlags.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	namespace VkUtils
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

	}
}
