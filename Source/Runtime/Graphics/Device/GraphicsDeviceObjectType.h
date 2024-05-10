#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API GraphicsDeviceObjectType : byte
	{
		Swapchain,
		Texture,
		TextureView,
		GraphicsBuffer,
		GraphicsMemory,
		RenderPass,
		Shader,
		Sampler,
		Pipeline,
		DescriptorSet,
		DescriptorPool,
		DescriptorLayout,
		CommandBuffer,
		CommandPool,
		Fence,
		Semaphore,
		Queue
	};
}
