#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	/**
	 * @enum CommandPoolType
	 * @brief Enumerates the types of command pools
	 */
	enum class RUNTIME_API CommandPoolType : byte
	{
		Graphics = 1,
		Compute = 2,
		Transfer = 4
	};
}