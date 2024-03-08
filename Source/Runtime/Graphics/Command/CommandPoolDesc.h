#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Command/CommandPoolType.h>

namespace Hollow
{
	struct RUNTIME_API CommandPoolDesc
	{
		CommandPoolType PoolType;
	};
}