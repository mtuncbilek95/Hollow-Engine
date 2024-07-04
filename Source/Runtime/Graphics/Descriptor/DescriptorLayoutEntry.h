#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>
#include <Runtime/Graphics/Descriptor/DescriptorType.h>
#include <Runtime/Graphics/Descriptor/DescriptorSetFlags.h>

namespace Hollow
{
	struct RUNTIME_API DescriptorLayoutEntry
	{
		u32 Binding; 
		DescriptorType Type;
		ShaderStage ShaderStages;
		DescriptorSetFlags Flags;
	};
}