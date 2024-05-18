#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryAccessFlags.h>
#include <Runtime/Graphics/Pipeline/PipelineStageFlags.h>
namespace Hollow
{
	struct RUNTIME_API CommonMemoryBarrierUpdateDesc
	{
		PipelineStageFlags SourceStageFlags;
		GraphicsMemoryAccessFlags SourceAccessMask;

		PipelineStageFlags DestinationStageFlags;
		GraphicsMemoryAccessFlags DestinationAccessMask;
	};
}
