#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	struct ENGINE_API ViewportDesc
	{
		Vec2u ViewportSize;
		Vec2u OffsetSize;
		Vec2f DepthRange;
	};
}