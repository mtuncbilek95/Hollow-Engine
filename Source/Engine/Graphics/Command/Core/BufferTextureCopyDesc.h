#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	/**
	 * @struct BufferTextureCopyDesc
	 * @brief Mostly used for copying between host texture data and device buffers
	 */
	struct BufferTextureCopyDesc
	{
		u64 BufferOffsetInBytes;
		Vec3i TextureOffset;
		Vec3u TextureSize;
		u8 TargetArrayIndex;
		u8 TargetMipIndex;
	};
}