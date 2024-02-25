#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API TextureUsage : byte
	{
		Sampled,
		Storage,
		ColorAttachment,
		DepthStencilAttachment,
		TransientAttachment,
		InputAttachment,
		TransferSource,
		TransferDestination
	};

	GENERATE_ENUM_FLAG(TextureUsage, byte);
}