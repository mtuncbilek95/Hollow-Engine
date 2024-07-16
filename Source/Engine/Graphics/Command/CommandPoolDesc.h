#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Command/CommandPoolType.h>

namespace Hollow
{
	/**
	 * @struct CommandPoolDesc
	 * @brief Description of a command pool
	 */
	struct ENGINE_API CommandPoolDesc
	{
		CommandPoolType PoolType;
	};
}