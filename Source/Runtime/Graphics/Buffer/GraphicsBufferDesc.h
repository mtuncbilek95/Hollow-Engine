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
		uint32 SizeInBytes;
		uint64 StructureByteStride;

		SharedPtr<GraphicsMemory> pMemory;
	};
}
