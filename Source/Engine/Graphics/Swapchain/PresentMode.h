#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class ENGINE_API PresentMode : u8
	{
		Immediate = 0, // VK_IMMEDIATE
		VSyncImmediate = 1, // VK_FIFO_RELAXED
		VSyncQueued = 2, // VK_MAILBOX
		FullVSync = 3// VK_FIFO
	};
}