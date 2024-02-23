#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Graphics/Common/GraphicsAPI.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsInstanceDesc
	{
		GraphicsAPI API = GraphicsAPI::Vulkan;
		Array<String> EnabledExtensions;
	};
}
