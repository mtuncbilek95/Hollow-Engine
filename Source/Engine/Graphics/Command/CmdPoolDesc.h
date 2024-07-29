#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Command/CmdPoolType.h>

namespace Hollow
{
	/**
	 * @struct CommandPoolDesc
	 * @brief Description of a command pool
	 */
	struct CmdPoolDesc
	{
		CmdPoolType PoolType;
	};
}