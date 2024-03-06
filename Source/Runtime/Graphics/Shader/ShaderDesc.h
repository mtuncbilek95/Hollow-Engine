#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>

namespace Hollow
{
	struct RUNTIME_API ShaderDesc
	{
		String ShaderName;
		String Source;
		ShaderStage Type;
		String EntryPoint;
		String ShaderModel;
	};
}