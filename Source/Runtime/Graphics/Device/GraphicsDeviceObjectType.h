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
		Shader,
		Sampler,
		Pipeline,
		ResourceLayout
	};
}
