#include "ShaderCompiler.h"

#include <Runtime/ShaderCompiler/ShaderCompilerUtils.h>

#include <spirv_cross/spirv_hlsl.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <shaderc/shaderc.hpp>

#include <spirv_reflect.h>

#include <Runtime/Platform/PlatformDirectory.h>
#include <Runtime/Platform/PlatformAPI.h>

namespace Hollow
{
	bool ShaderCompiler::CompileShaderToSPIRV(const MemoryBuffer& view, const String& entryMethodName, const ShaderStage stage,
		const ShaderLanguage language, MemoryOwnedBuffer& pViewOut,
		String& errorMessageOut)
	{
		shaderc::Compiler vkCompiler;
		shaderc::CompileOptions compileOptions;

		compileOptions.SetIncluder(MakeOwned<ShaderIncluder>());
		compileOptions.SetSourceLanguage(ShaderCompilerUtils::GetLanguage(language));
		compileOptions.SetSuppressWarnings();

		shaderc::PreprocessedSourceCompilationResult preprocessedResult = vkCompiler.PreprocessGlsl(static_cast<const char*>(view.GetData()),
			ShaderCompilerUtils::GetShaderKind(stage), entryMethodName.c_str(), compileOptions);

		if (preprocessedResult.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			errorMessageOut = preprocessedResult.GetErrorMessage();
			CORE_ASSERT(false, "ShaderCompiler", "Failed to preprocess shader. Reason: %s", errorMessageOut.c_str());
			return false;
		}

		shaderc::CompilationResult result = vkCompiler.CompileGlslToSpv(static_cast<const char*>(preprocessedResult.begin()), ShaderCompilerUtils::GetShaderKind(stage), "", compileOptions);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			errorMessageOut = result.GetErrorMessage();
			CORE_ASSERT(false, "ShaderCompiler", "Failed to compile shader to SPIRV. Reason: %s", errorMessageOut.c_str());
			return false;
		}

		const u64 size = (byte*)result.end() - (byte*)result.begin();
		byte* data = new byte[size];
		memcpy(data, result.begin(), size);

		pViewOut = MemoryOwnedBuffer(std::move(data), size);
		CORE_ASSERT(pViewOut.GetSize() > 0, "ShaderCompiler", "Failed to compile shader to SPIRV");
		return true;
	}
}
