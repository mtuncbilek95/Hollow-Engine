#pragma once 

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Buffer/GraphicsBufferUsage.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryType.h>
#include <Runtime/Graphics/Common/ShareMode.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsBufferDesc
	{
		uint64 SizeInBytes;
		GraphicsBufferUsage Usage;
		GraphicsMemoryType MemoryType;
		ShareMode ShareMode;
	};
}