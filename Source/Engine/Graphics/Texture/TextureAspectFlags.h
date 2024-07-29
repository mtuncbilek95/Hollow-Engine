#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class TextureAspectFlags : u8
	{
		Undefined = 0,
		ColorAspect = 1 << 0,
		DepthAspect = 1 << 1,
		StencilAspect = 1 << 2,
		MetaAspect = 1 << 3
	};

	GENERATE_ENUM_FLAG(TextureAspectFlags, u8);
}
