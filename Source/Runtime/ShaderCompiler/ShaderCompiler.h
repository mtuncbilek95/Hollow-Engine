#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Memory/MemoryBuffer.h>
#include <Runtime/Memory/MemoryOwnedBuffer.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>
#include <Runtime/Graphics/Shader/ShaderLanguage.h>
#include <Runtime/ShaderCompiler/ShaderIncluder.h>

namespace Hollow
{
	class RUNTIME_API ShaderCompiler
	{
	public:
		static bool CompileShaderToSPIRV(const MemoryBuffer& view, const String& entryMethodName, const ShaderStage stage, const ShaderLanguage language,
			MemoryOwnedBuffer& pViewOut, String& errorMessageOut);
	public:
		ShaderCompiler() = delete;
		~ShaderCompiler() = delete;
	};
}