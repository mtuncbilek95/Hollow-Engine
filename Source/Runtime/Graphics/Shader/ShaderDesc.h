#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Shader/ShaderType.h>

namespace Hollow
{
	struct RUNTIME_API ShaderDesc
	{
		String ShaderName;
		String Source;
		ShaderType Type;
		String EntryPoint;
		String ShaderModel;
	};
}
