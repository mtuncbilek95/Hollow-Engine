#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API ApplicationModuleState
	{
		Unknown,
		NeedValidation,
		Validated,
		Invalidated
	};
}
