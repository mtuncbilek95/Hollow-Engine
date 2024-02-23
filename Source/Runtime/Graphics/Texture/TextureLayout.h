#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API TextureLayout : byte
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