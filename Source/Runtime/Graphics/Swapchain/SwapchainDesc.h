#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Swapchain/PresentMode.h>

namespace Hollow
{
	class GraphicsQueue;
	struct RUNTIME_API SwapchainDesc
	{
		Vector2u ImageSize;
		TextureFormat SwapchainFormat;
		PresentMode VSync;
		byte BufferCount;
		byte SampleCount;
		GraphicsQueue* pQueue;
		
	};
}
       