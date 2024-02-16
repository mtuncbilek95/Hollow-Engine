#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API ResourceUsageType : byte
	{
		Default,
		Immutable,
		Dynamic,
		Staging
	};
}
