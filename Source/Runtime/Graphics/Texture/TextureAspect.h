#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API TextureAspect : byte
	{
		TextureColor,
		TextureDepth,
		TextureStencil,
	};
}