#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	/**
	 * @struct BufferBufferCopyDesc
	 * @brief Mostly used for copying between host and device buffers
	 */
	struct ENGINE_API BufferBufferCopyDesc
	{
		u64 SourceOffset;
		u64 DestinationOffset;
		u64 Size;
	};
}