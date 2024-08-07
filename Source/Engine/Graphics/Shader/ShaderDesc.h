#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Shader/ShaderStage.h>
#include <Engine/Memory/MemoryBuffer.h>

namespace Hollow
{
	struct ShaderDesc
	{
		String ShaderName;
		String Entry;
		ShaderStage Stage;
		MemoryBuffer ShaderCode;
	};
}
