#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Memory/MemoryBuffer.h>

namespace Hollow
{
	/**
	 * @struct BufferDataUpdateDesc
	 * @brief Mostly used to update the data of host buffer
	 */
	struct RUNTIME_API BufferDataUpdateDesc
	{
		MemoryBuffer Memory = {};
		uint32 OffsetInBytes = 0;
	};
}