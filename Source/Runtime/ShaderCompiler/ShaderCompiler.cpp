#include "ShaderCompiler.h"

#include <Runtime/ShaderCompiler/ShaderCompilerUtils.h>

#include <spirv_cross/spirv_hlsl.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <shaderc/shaderc.hpp>

namespace Hollow
{
	bool ShaderCompiler::CompileShaderToSPIRV(const MemoryBuffer& view, const String& entryMethodName, const ShaderStage stage, const ShaderLanguage language, SharedPtr<MemoryOwnedBuffer>& pViewOut, String& errorMessageOut)
	{
		shaderc::Compiler vkCompiler;
		shaderc::CompileOptions compileOptions;

		// Set the source language
		compileOptions.SetSourceLanguage(ShaderCompilerUtils::GetLanguage(language));
		compileOptions.SetSuppressWarnings();

		shaderc::PreprocessedSourceCompilationResult preprocessedResult = vkCompiler.PreprocessGlsl(static_cast<const char*>(view.GetBuffer()),
			ShaderCompilerUtils::GetShaderKind(stage), entryMethodName.c_str(), compileOptions);

		if (preprocessedResult.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			errorMessageOut = preprocessedResult.GetErrorMessage();
			pViewOut = nullptr;
			return false;
		}

		shaderc::CompilationResult result = vkCompiler.CompileGlslToSpv(static_cast<const char*>(view.GetBuffer()), ShaderCompilerUtils::GetShaderKind(stage), "", compileOptions);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			errorMessageOut = result.GetErrorMessage();
			pViewOut = nullptr;
			return false;
		}

		const uint64 size = (byte*)result.end() - (byte*)result.begin();
		byte* data = new byte[size];
		memcpy(data, result.begin(), size);

		pViewOut = std::make_shared<MemoryOwnedBuffer>(std::move(data), size);
		CORE_ASSERT(pViewOut->GetSize() > 0, "ShaderCompiler", "Failed to compile shader to SPIRV");

		return true;
	}
}
