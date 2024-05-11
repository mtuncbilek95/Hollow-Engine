#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class GraphicsQueueType : byte
	{
		Graphics = 1,
		Compute = 2,
		Transfer = 4
	};
}
