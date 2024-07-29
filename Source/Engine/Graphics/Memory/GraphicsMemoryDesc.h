#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Memory/GraphicsMemoryType.h>

namespace Hollow
{
	struct GraphicsMemoryDesc
	{
		GraphicsMemoryType MemoryType;
		u64 MemorySize;
	};
}
