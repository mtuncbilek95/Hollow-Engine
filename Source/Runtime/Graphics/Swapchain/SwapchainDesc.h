#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>

namespace Hollow
{
	class GraphicsInstance;
	struct RUNTIME_API SwapchainDesc
	{
		TextureFormat SwapchainFormat;
		bool VSync;
		bool Windowed;
		byte BufferCount;
		byte SampleCount;
		SharedPtr<GraphicsInstance> pInstance;
	};
}
       