#pragma once

namespace Hollow
{
	enum class RUNTIME_API PresentMode : byte
	{
		Immediate, // VK_IMMEDIATE
		VSyncImmediate, // VK_FIFO_RELAXED
		VSyncQueued, // VK_MAILBOX
		FullVSync // VK_FIFO
	};
}