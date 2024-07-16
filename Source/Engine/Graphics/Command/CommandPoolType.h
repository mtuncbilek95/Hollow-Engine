#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class ENGINE_API CommandPoolType : u8
	{
		Graphics = 1,
		Compute = 2,
		Transfer = 4
	};
}