#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API TextureUsage : byte
	{
		None = 0,
		Sampled = 1,
		Storage = 2,
		ColorAttachment = 4,
		DepthStencilAttachment = 8,
		TransientAttachment = 16,
		InputAttachment = 32,
		TransferSource = 64,
		TransferDestination = 128
	};

	GENERATE_ENUM_FLAG(TextureUsage, byte);
}