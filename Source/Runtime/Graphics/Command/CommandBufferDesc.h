#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	class CommandPool;
	struct RUNTIME_API CommandBufferDesc
	{
		CommandPool* pCommandPool;
	};
}
