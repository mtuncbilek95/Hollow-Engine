#pragma once 

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Buffer/GraphicsBufferUsage.h>

namespace Hollow
{
	class ENGINE_API GraphicsMemory;
	struct ENGINE_API GraphicsBufferDesc
	{
		u64 BlockSize;
		u64 BlockLength;
		GraphicsBufferUsage Usage;
		SharedPtr<GraphicsMemory> pRequestMemory;
	};
}