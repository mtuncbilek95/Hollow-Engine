#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class GraphicsMemoryAccessFlags : u32
	{
		Unknown = 0,
		IndexRead = 1 << 0,
		VertexAttributeRead = 1 << 1,
		UniformRead = 1 << 2,
		InputAttachmentRead = 1 << 3,
		ShaderRead = 1 << 4,
		ShaderWrite = 1 << 5,
		ColorAttachmentRead = 1 << 6,
		ColorAttachmentWrite = 1 << 7,
		DepthStencilAttachmentRead = 1 << 8,
		DepthStencilAttachmentWrite = 1 << 9,
		TransferRead = 1 << 10,
		TransferWrite = 1 << 11,
		HostRead = 1 << 12,
		HostWrite = 1 << 13,
		MemoryRead = 1 << 14,
		MemoryWrite = 1 << 15
	};

	GENERATE_ENUM_FLAG(GraphicsMemoryAccessFlags, u32);
}
