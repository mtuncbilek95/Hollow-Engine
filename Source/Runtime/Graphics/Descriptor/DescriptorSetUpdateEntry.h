#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorType.h>

namespace Hollow
{
	struct RUNTIME_API DescriptorSetUpdateEntry
	{
		SharedPtr<GraphicsDeviceObject> pResource;
		SharedPtr<GraphicsDeviceObject> pSampler;
		DescriptorType Type;
		u32 Count;
		u32 ArrayElement;
		u64 BufferOffset;
		u32 Binding;
	};
}