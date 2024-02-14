#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API SemanticName : byte
	{
		Position,
		Normal,
		TexCoord,
		Tangent,
		BiNormal,
		Color,
		Count
	};
}
