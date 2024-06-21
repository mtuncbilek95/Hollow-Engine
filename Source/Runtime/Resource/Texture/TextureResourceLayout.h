#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Memory/MemoryOwnedBuffer.h>

namespace Hollow
{
	struct RUNTIME_API TextureResourceLayout
	{
		Vector2i ImageSize;
		MemoryOwnedBuffer ImageData;
		i32 Channels;
	};
}
