#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API BufferDataUpdateDesc
	{
		void* pData;
		uint32 SizeInBytes;
	};
}