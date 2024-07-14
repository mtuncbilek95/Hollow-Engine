#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Memory/GraphicsMemoryType.h>

namespace Hollow
{
	struct ENGINE_API GraphicsMemoryDesc
	{
		GraphicsMemoryType MemoryType;
		u64 MemorySize;
	};
}
