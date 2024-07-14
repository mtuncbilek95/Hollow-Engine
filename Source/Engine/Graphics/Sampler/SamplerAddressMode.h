#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class ENGINE_API SamplerAddressMode : u8
	{
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder,
		MirrorClampToEdge
	};
}
