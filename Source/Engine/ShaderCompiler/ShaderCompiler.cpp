#include "ShaderCompiler.h"
#include "ShaderIncluder.h"

#include <Engine/Platform/PlatformFile.h>

#include <shaderc/shaderc.hpp>
#include <stdexcept>

namespace Hollow::ShaderCompiler
{
	MemoryOwnedBuffer CompileGLSLToSPIRV(const String& Path, const String& Entry)
	{
		String sourceCode;

		if (!PlatformFile::Read(Path, sourceCode))
			throw std::runtime_error("Failed to read shader file: " + Path);

		shaderc::Compiler shaderCompiler;
		shaderc::CompileOptions options;

		options.SetIncluder(MakeOwned<ShaderIncluder>());
		options.SetSourceLanguage(shaderc_source_language_glsl);
		options.SetGenerateDebugInfo();
		options.SetOptimizationLevel(shaderc_optimization_level_performance);
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		options.SetInvertY(true);

		shaderc::PreprocessedSourceCompilationResult preResult = shaderCompiler.PreprocessGlsl(sourceCode, shaderc_glsl_infer_from_source, Entry.c_str(), options);
		CORE_ASSERT(preResult.GetCompilationStatus() == shaderc_compilation_status_success, "SPIRV", "Failed to process shader: %s", preResult.GetErrorMessage().c_str());

		shaderc::CompilationResult<u32> result = shaderCompiler.CompileGlslToSpv(sourceCode, shaderc_glsl_infer_from_source, Entry.c_str(), options);
		CORE_ASSERT(result.GetCompilationStatus() == shaderc_compilation_status_success, "SPIRV", "Failed to compile shader: %s", result.GetErrorMessage().c_str());

		const u32 size = result.cend() - result.cbegin();
		u8* data = new u8[size];
		memcpy(data, result.begin(), size);

		return MemoryOwnedBuffer(std::move(data), size);
	}
}
