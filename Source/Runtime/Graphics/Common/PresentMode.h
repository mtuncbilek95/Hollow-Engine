#pragma once

namespace Hollow
{
	enum class RUNTIME_API PresentMode : byte
	{
		Immediate,
		VSyncImmediate,
		VSyncQueued,
		FullVSync
	};
}