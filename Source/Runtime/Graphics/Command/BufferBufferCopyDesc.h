#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API BufferBufferCopyDesc
	{
		uint64 SourceOffsetInBytes;
		uint64 DestinationOffsetInBytes;
		uint64 SizeInBytes;
	};
}