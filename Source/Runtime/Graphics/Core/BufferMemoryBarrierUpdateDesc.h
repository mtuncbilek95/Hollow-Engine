#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <Runtime/Graphics/Pipeline/PipelineStageFlags.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryAccessFlags.h>
#include <Runtime/Graphics/Queue/GraphicsQueueType.h>

namespace Hollow
{
	struct RUNTIME_API BufferMemoryBarrierUpdateDesc
	{
		PipelineStageFlags SourceStageFlags;
		GraphicsMemoryAccessFlags SourceAccessMask;
		GraphicsQueueType SourceQueue;

		PipelineStageFlags DestinationStageFlags;
		GraphicsMemoryAccessFlags DestinationAccessMask;
		GraphicsQueueType DestinationQueue;

		SharedPtr<GraphicsBuffer> pBuffer;
		u64 Offset;
		u64 Size;
	};
}
