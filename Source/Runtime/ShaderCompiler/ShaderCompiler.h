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
			const ShaderLanguage language, uint32** ppBytesOut, uint32& bytesSizeOut, String& errorMessageOut);

		static String GetShaderCodeAsString(const String& filePath);
	public:
		ShaderCompiler() = delete;
		~ShaderCompiler() = delete;
	};
}