#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API TextureAspectFlags : byte
	{
		None = 0,
		ColorAspect = 1,
		DepthAspect = 2,
		StencilAspect = 4,
		MetaAspect = 8
	};

	GENERATE_ENUM_FLAG(TextureAspectFlags, byte);
}