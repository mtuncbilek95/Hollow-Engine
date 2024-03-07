#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API TextureMemoryLayout : byte
	{
		Unknown,
		General,
		ColorAttachment,
		DepthStencilAttachment,
		ShaderReadOnly,
		TransferSource,
		TransferDestination,
		DepthAttachment,
		Present
	};
}