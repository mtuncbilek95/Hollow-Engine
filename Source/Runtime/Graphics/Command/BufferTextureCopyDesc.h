#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API BufferTextureCopyDesc
	{
		uint64 BufferOffsetInBytes;
		uint32 TextureOffsetX;
		uint32 TextureOffsetY;
		uint32 TextureOffsetZ;
		uint32 Width;
		uint32 Height;
		uint32 Depth;
		byte TargetArrayIndex;
		byte TargetMipIndex;
	};
}