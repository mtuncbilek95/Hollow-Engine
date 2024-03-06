#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Graphics/Device/GraphicsAPI.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsInstanceDesc
	{
		String InstanceName;
		String ApplicationName;
		GraphicsAPI API = GraphicsAPI::Vulkan;
		Array<String> EnabledExtensions;
	};
}