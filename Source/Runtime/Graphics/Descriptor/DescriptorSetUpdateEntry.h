#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorType.h>

namespace Hollow
{
	struct RUNTIME_API DescriptorSetUpdateEntry
	{
		GraphicsDeviceObject* pResource;
		DescriptorType Type;
		uint32 Count;
		uint32 ArrayElement;
		uint64 BufferOffset;
		uint32 Binding;
	};
}