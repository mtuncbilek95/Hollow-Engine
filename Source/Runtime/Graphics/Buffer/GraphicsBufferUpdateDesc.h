#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsBufferUpdateDesc
	{
		void* pData;
		uint32 SizeInBytes;
	};
}
