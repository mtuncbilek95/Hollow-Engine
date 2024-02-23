#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureAspectFlags.h>
#include <Runtime/Graphics/Sampler/SamplerFiltering.h>

namespace Hollow
{
	struct RUNTIME_API TextureTextureCopyDesc
	{
		Array<TextureAspectFlags> SourceAspect;
		uint32 SourceOffsetX;
		uint32 SourceOffsetY;
		uint32 SourceOffsetZ;
		byte SourceArrayIndex;
		byte SourceMipIndex;

		Array<TextureAspectFlags> DestinationAspect;
		uint32 DestinationOffsetX;
		uint32 DestinationOffsetY;
		uint32 DestinationOffsetZ;
		byte DestinationArrayIndex;
		byte DestinationMipIndex;

		SamplerFiltering Filtering;
	};
}