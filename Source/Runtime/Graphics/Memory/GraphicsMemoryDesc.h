#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryType.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsMemoryDesc
	{
		GraphicsMemoryType MemoryType;
		GraphicsBuffer* pOwnerBuffer;
	};
}