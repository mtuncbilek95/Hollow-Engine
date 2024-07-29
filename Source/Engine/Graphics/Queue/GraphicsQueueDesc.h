#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Queue/GraphicsQueueType.h>

namespace Hollow
{
	struct GraphicsQueueDesc
	{
		GraphicsQueueType QueueType;
		u32 QueueIndex;
	};
}
