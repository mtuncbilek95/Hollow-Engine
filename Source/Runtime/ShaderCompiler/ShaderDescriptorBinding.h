#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/ShaderCompiler/ShaderDescriptorBindingType.h>
#include <Runtime/ShaderCompiler/ShaderBlockMember.h>

namespace Hollow
{
	struct RUNTIME_API ShaderDescriptorBinding
	{
		u32 BindIndex;
		ShaderDescriptorBindingType Type;
		String Name;
		ShaderBlockMember BlockMember;
	};
}
