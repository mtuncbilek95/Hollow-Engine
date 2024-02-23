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
		Array<TextureAspectFlags> AspectFlags;
		byte ArrayIndex;
		byte MipIndex;

		TextureLayout SourceLayout;
		Array<GraphicsMemoryAccessFlags> SourceAccessFlags;
		Array<PipelineStageFlags> SourceStageFlags;
		GraphicsQueueType SourceQueue;

		TextureLayout DestinationLayout;
		Array<GraphicsMemoryAccessFlags> DestinationAccessFlags;
		Array<PipelineStageFlags> DestinationStageFlags;
		GraphicsQueueType DestinationQueue;
	};
}
