#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Memory/MemoryBuffer.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>
#include <Runtime/Graphics/Shader/ShaderLanguage.h>

namespace Hollow
{
	struct RUNTIME_API ShaderDesc
	{
		String ShaderName;
		String EntryPoint;
		MemoryBuffer ShaderCode;
		ShaderStage Stage;
		ShaderLanguage Language;
	};
}