#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class ENGINE_API PresentMode : u8
	{
		Immediate, // VK_IMMEDIATE
		VSyncImmediate, // VK_FIFO_RELAXED
		VSyncQueued, // VK_MAILBOX
		FullVSync // VK_FIFO
	};
}