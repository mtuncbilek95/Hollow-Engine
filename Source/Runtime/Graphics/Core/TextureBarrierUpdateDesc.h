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
	/**
	 * @struct TextureBarrierUpdateDesc
	 * @brief Describes a texture barrier update. Which is being used to update the texture 
	 * layout and access flags before using the texture in a different stages.
	 */
	struct RUNTIME_API TextureBarrierUpdateDesc
	{
		GraphicsMemoryAccessFlags SourceAccessMask;
		TextureMemoryLayout OldLayout;
		PipelineStageFlags SourceStageFlags;
		GraphicsQueueType SourceQueue;

		GraphicsMemoryAccessFlags DestinationAccessMask;
		TextureMemoryLayout NewLayout;
		PipelineStageFlags DestinationStageFlags;
		GraphicsQueueType DestinationQueue;

		SharedPtr<Texture> pTexture;

		TextureAspectFlags AspectMask;
		uint32 MipIndex;
		uint32 ArrayIndex;
	};
}