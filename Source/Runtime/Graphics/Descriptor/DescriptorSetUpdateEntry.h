#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>

#include <Runtime/Graphics/Descriptor/DescriptorResourceType.h>

namespace Hollow
{
	struct RUNTIME_API DescriptorSetUpdateEntry
	{
		SharedPtr<GraphicsDeviceObject> pResource;
		DescriptorResourceType Type;
		uint32 Count;
		uint32 ArrayElement;
		uint64 BufferOffsetInBytes;
		uint32 Binding;
	};
}
