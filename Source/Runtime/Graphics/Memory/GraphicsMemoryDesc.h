#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryType.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsMemoryDesc
	{
		GraphicsMemoryType MemoryType;
		uint64 SizeInBytes;
	};
}