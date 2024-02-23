#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsBufferUpdateDesc
	{
		byte* pData;
		uint64 SizeInBytes;
		uint64 OffsetInBytes;
	};
}
