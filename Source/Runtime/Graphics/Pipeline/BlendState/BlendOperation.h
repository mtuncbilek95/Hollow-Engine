#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API BlendOperation : byte
	{
		Add,
		Subtract,
		ReverseSubtract,
		Min,
		Max
	};
}
