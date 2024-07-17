#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Texture/TextureFormat.h>
#include <Engine/Graphics/Texture/TextureUsageFlags.h>
#include <Engine/Graphics/Texture/TextureType.h>
#include <Engine/Graphics/Swapchain/PresentMode.h>

namespace Hollow
{
	class ENGINE_API GraphicsQueue;
	struct ENGINE_API SwapchainDesc
	{
		Vec2u ImageSize;
		TextureFormat ImageFormat;
		TextureUsageFlags TextureUsage;
		PresentMode VSync;
		u8 BufferCount;
		WeakPtr<GraphicsQueue> pRequestQueue;
	};
}