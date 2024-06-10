#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Memory/MemoryBuffer.h>

namespace Hollow
{
	struct RUNTIME_API TextureResourceLayout
	{
		Vector2i ImageSize;
		MemoryBuffer ImageData;
		i32 Channels;
	};
}
