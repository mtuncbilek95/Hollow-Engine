#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API BufferCPUAccess : byte
	{
		None,
		Read,
		Write
	};
}
