#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API TextureAspectFlags : byte
	{
		ColorAspect = 1,
		DepthAspect = 2,
		StencilAspect = 4
	};

	GENERATE_ENUM_FLAG(TextureAspectFlags, byte);
}