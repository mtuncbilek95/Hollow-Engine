#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Memory/GraphicsMemoryAccessFlags.h>
#include <Engine/Graphics/Texture/TextureLayout.h>
#include <Engine/Graphics/Texture/TextureAspectFlags.h>
#include <Engine/Graphics/Pipeline/PipelineStageFlags.h>
#include <Engine/Graphics/Queue/GraphicsQueueType.h>

namespace Hollow
{
	struct TextureImageBarrier
	{
		GraphicsMemoryAccessFlags SourceAccessMask;
		TextureLayout OldLayout;
		PipelineStageFlags SourceStageFlags;
		GraphicsQueueType SourceQueue;

		GraphicsMemoryAccessFlags DestinationAccessMask;
		TextureLayout NewLayout;
		PipelineStageFlags DestinationStageFlags;
		GraphicsQueueType DestinationQueue;

		TextureAspectFlags AspectMask;
		u32 MipIndex;
		u32 ArrayIndex;
	};
}