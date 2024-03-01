#pragma once

#include <Core/Core.h>
#include <Shader/Shader.h>

namespace MiniVk
{
	class ShaderCompiler
	{
	public:
		static bool CompileShaderToSPIRV(const String& Source, const String& entryMethod, const ShaderStage stage,
			const ShaderLanguage language, uint32** ppBytesOut, uint32& bytesSizeOut, String& errorMessageOut);

		static String ReadFile(const String& filePath);
	public:
		ShaderCompiler() = delete;
		~ShaderCompiler() = delete;
	};
}