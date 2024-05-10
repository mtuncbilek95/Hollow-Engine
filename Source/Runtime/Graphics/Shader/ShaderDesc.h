#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>
#include <Runtime/Graphics/Shader/ShaderLanguage.h>

namespace Hollow
{
	struct RUNTIME_API ShaderDesc
	{
		string ShaderName;
		string Source;
		ShaderStage Stage;
		ShaderLanguage Language;
		string EntryPoint;
	};
}