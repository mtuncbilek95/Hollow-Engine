#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Buffer/BufferUsage.h>
#include <Runtime/Graphics/Buffer/BufferCPUAccess.h>
#include <Runtime/Graphics/Buffer/ResourceUsageType.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsViewDesc
	{
		BufferUsage Usage;
		BufferCPUAccess CPUAccess;
		ResourceUsageType UsageType;
		uint8 MiscFlags;
		uint32 SizeInBytes;
		uint64 StructureByteStride;

		void* pInitialData;
	};
}
