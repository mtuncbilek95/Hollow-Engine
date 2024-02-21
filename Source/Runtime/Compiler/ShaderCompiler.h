#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>
#include <Runtime/Graphics/Shader/ShaderLanguage.h>

namespace Hollow
{
	class RUNTIME_API ShaderCompiler
	{
	public:
		static bool CompileShaderToSPIRV(const String& Source, const String& entryMethod, const ShaderStage stage,
			const ShaderLanguage language, byte** ppBytesOut, uint32& bytesSizeOut, String& errorMessageOut);

	public:
		ShaderCompiler() = delete;
		~ShaderCompiler() = delete;
	};
}