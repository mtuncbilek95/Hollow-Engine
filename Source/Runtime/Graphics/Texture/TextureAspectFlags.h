#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API TextureAspectFlags : byte
	{
		None,
		ColorAspect,
		DepthAspect,
		StencilAspect
	};

	GENERATE_ENUM_FLAG(TextureAspectFlags, byte);
}