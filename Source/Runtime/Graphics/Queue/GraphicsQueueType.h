#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API GraphicsQueueType : byte
	{
		Graphics,
		Compute,
		Transfer
	};
}
