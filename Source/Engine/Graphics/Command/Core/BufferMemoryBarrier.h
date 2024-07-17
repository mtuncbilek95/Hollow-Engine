#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Buffer/GraphicsBuffer.h>
#include <Engine/Graphics/Pipeline/PipelineStageFlags.h>
#include <Engine/Graphics/Memory/GraphicsMemoryAccessFlags.h>
#include <Engine/Graphics/Queue/GraphicsQueueType.h>

namespace Hollow
{
	struct ENGINE_API BufferMemoryBarrier
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
