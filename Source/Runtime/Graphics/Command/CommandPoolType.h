#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API CommandPoolType : byte
	{
		Graphics,
		Compute,
		Transfer
	};
}