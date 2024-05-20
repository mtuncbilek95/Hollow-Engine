#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API GraphicsDeviceObjectType : byte
	{
		Unknown,
		Swapchain,
		GraphicsMemory,
		Texture,
		TextureBuffer,
		Sampler,
		GraphicsBuffer,
		CommandBuffer,
		CommandPool,
		DescriptorSet,
		DescriptorPool,
		DescriptorLayout,
		DynamicPass,
		Shader,
		Pipeline,
		Semaphore,
		Fence,
		Queue
	};
}
