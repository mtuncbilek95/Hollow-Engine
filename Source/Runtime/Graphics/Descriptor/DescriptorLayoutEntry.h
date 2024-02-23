#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Graphics/Descriptor/DescriptorResourceType.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>

namespace Hollow
{
	struct RUNTIME_API DescriptorLayoutEntry
	{
		DescriptorResourceType Type;
		ShaderStage Stage;
		uint32 Binding;
	};
}