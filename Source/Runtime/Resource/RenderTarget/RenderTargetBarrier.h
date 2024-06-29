#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryAccessFlags.h>
#include <Runtime/Graphics/Texture/TextureMemoryLayout.h>
#include <Runtime/Graphics/Texture/TextureAspectFlags.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Pipeline/PipelineStageFlags.h>
#include <Runtime/Graphics/Queue/GraphicsQueueType.h>

namespace Hollow
{
	struct RUNTIME_API RenderTargetBarrier
	{
		GraphicsMemoryAccessFlags DestinationAccessMask;
		TextureMemoryLayout NewLayout;
		PipelineStageFlags DestinationStageFlags;
		GraphicsQueueType DestinationQueue;

		TextureAspectFlags AspectMask;
	};
}
