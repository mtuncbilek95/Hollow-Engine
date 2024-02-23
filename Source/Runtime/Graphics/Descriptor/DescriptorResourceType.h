#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API DescriptorResourceType : byte
	{
		Sampler,
		SampledTexture,
		StorageTexture,
		ConstantBuffer,
		StorageBuffer
	};
}