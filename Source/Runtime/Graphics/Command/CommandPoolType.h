#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API CommandPoolType : byte
	{
		Graphics = 1,
		Compute = 2,
		Transfer = 4
	};
}