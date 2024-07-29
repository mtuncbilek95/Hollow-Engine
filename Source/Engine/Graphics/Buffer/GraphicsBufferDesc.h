#pragma once 

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Buffer/GraphicsBufferUsage.h>

namespace Hollow
{
	class GraphicsMemory;
	struct GraphicsBufferDesc
	{
		u64 BlockSize;
		u64 BlockLength;
		GraphicsBufferUsage Usage;
		WeakPtr<GraphicsMemory> pRequestMemory;
	};
}