#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Queue/GraphicsQueueType.h>

namespace Hollow
{
	struct ENGINE_API GraphicsQueueDesc
	{
		GraphicsQueueType QueueType;
		u32 QueueIndex;
	};
}
