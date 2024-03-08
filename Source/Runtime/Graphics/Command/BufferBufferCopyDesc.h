#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API BufferBufferCopyDesc
	{
		uint64 SourceOffset;
		uint64 DestinationOffset;
		uint64 Size;
	};
}