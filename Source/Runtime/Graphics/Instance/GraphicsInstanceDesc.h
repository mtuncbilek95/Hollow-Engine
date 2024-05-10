#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsAPI.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsInstanceDesc
	{
		string InstanceName;
		string ApplicationName;
		GraphicsAPI API = GraphicsAPI::Vulkan;
		arrayList<string> EnabledExtensions;
	};
}