#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Texture/TextureUsage.h>
#include <Runtime/Graphics/Common/PresentMode.h>
#include <Runtime/Graphics/Common/ShareMode.h>

namespace Hollow
{
	class GraphicsQueue;
	struct RUNTIME_API SwapchainDesc
	{
		Vector2u ImageSize;
		TextureFormat SwapchainImageFormat;
		TextureUsage SwapchainUsage;
		PresentMode VSync;
		byte BufferCount;
		GraphicsQueue* pQueue;
		ShareMode SwapchainMode;
	};
}