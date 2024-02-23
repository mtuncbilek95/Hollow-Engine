#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API TextureUsage
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
}