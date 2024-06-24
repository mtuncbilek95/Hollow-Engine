#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API ViewportDesc
	{
		Vec2u ViewportSize;
		Vec2u OffsetSize;
		Vec2f DepthRange;
	};
}