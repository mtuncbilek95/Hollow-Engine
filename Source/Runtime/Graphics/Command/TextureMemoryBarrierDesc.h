#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureLayout.h>
#include <Runtime/Graphics/Texture/TextureAspectFlags.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryAccessFlags.h>
#include <Runtime/Graphics/Queue/GraphicsQueueType.h>
#include <Runtime/Graphics/Pipeline/PipelineStageFlags.h>

namespace Hollow
{
	struct RUNTIME_API TextureMemoryBarrierDesc
	{
		TextureAspectFlags AspectFlags;
		byte ArrayIndex;
		byte MipIndex;

		TextureLayout SourceLayout;
		GraphicsMemoryAccessFlags SourceAccessFlags;
		PipelineStageFlags SourceStageFlags;
		GraphicsQueueType SourceQueue;

		TextureLayout DestinationLayout;
		GraphicsMemoryAccessFlags DestinationAccessFlags;
		PipelineStageFlags DestinationStageFlags;
		GraphicsQueueType DestinationQueue;
	};
}
