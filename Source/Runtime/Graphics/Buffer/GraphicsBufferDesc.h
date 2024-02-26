#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Buffer/GraphicsBufferUsage.h>


namespace Hollow
{
	class GraphicsMemory;
	
	struct RUNTIME_API GraphicsBufferDesc
	{
		GraphicsBufferUsage Usage;
		uint8 MiscFlags;
		uint32 SubSizeInBytes;
		uint32 SubCount;

		SharedPtr<GraphicsMemory> pMemory;
	};
}
