#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API PrimitiveMode : byte
	{
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		TriangleStrip
	};
}
