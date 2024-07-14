#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Shader/ShaderStage.h>
#include <Engine/Graphics/Descriptor/DescriptorType.h>
#include <Engine/Graphics/Descriptor/DescriptorSetFlags.h>

namespace Hollow
{
	struct ENGINE_API DescriptorLayoutEntry
	{
		u32 Binding; 
		DescriptorType Type;
		ShaderStage ShaderStages;
		DescriptorSetFlags Flags;
	};
}