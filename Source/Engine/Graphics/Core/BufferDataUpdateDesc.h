#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Memory/MemoryBuffer.h>

namespace Hollow
{
	/**
	 * @struct BufferDataUpdateDesc
	 * @brief Mostly used to update the data of host buffer
	 */
	struct BufferDataUpdateDesc
	{
		MemoryBuffer Memory = {};
		u32 OffsetInBytes = 0;
	};
}