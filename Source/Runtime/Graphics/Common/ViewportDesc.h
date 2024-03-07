#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API ViewportDesc
	{
		Vector2u ViewportSize;
		Vector2u OffsetSize;
		Vector2f DepthRange;
	};
}