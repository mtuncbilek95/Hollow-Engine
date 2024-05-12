#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/ShaderCompiler/ShaderDescriptorBinding.h>

namespace Hollow
{
	struct RUNTIME_API ShaderDescriptorSet
	{
		uint32 SetIndex;
		ArrayList<ShaderDescriptorBinding> Bindings;
	};
}
