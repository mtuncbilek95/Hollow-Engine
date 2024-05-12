#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API ShaderIOSemantic : byte
	{
		Position,
		Normal,
		Tangent,
		Bitangent,
		TexCoord,
		Color
	};
}
