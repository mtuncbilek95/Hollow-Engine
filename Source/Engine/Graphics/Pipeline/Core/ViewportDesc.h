#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	struct ViewportDesc
	{
		Vec2u ViewportSize;
		Vec2u OffsetSize;
		Vec2f DepthRange;
	};
}