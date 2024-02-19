#pragma once

namespace Hollow
{
	enum class RUNTIME_API GraphicsMemoryAccessFlags : byte
	{
		None,
		IndexRead,
		VertexAttributeRead,
		ConstantRead,
		InputAttachmentRead,
		ShaderRead,
		ShaderWrite,
		ColorAttachmentRead,
		ColorAttachmentWrite,
		DepthStencilAttachmentRead,
		DepthStencilAttachmentWrite,
		TransferRead,
		TransferWrite,
		HostRead,
		HostWrite,
		MemoryRead,
		MemoryWrite
	};
}
