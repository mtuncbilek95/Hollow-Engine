#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API TextureSampleCount : byte
	{
		Sample1 = 1,
		Sample2 = 2,
		Sample4 = 4,
		Sample8 = 8,
		Sample16 = 16,
		Sample32 = 32,
		Sample64 = 64
	};
}