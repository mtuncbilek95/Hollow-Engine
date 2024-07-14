#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Descriptor/DescriptorType.h>

namespace Hollow
{
	struct ENGINE_API DescriptorSetUpdateEntry
	{
		SharedPtr<DeviceObject> pResource;
		SharedPtr<DeviceObject> pSampler;
		DescriptorType Type;
		u32 Count;
		u32 ArrayElement;
		u64 BufferOffset;
		u32 Binding;
	};
}