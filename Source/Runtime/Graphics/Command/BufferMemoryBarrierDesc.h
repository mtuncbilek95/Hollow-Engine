#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/PipelineStageFlags.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryAccessFlags.h>
#include <Runtime/Graphics/Queue/GraphicsQueueType.h>

namespace Hollow
{
	struct RUNTIME_API BufferMemoryBarrierDesc
	{
		uint64 OffsetInBytes;
		uint64 SizeInBytes;

		Array<PipelineStageFlags> SourceStageFlags;
		Array<GraphicsMemoryAccessFlags> SourceAccessFlags;
		GraphicsQueueType SourceQueue;

		Array<PipelineStageFlags> DestinationStageFlags;
		Array<GraphicsMemoryAccessFlags> DestinationAccessFlags;
		GraphicsQueueType DestinationQueue;
	};
}
