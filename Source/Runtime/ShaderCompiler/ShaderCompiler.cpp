#include "ShaderCompiler.h"

#include <Runtime/ShaderCompiler/ShaderCompilerUtils.h>

#include <SPIRV-Cross/Include/spirv_hlsl.hpp>
#include <SPIRV-Cross/Include/spirv_glsl.hpp>
#include <Shaderc/Include/shaderc.hpp>

#include <fstream>

namespace Hollow
{
	bool ShaderCompiler::CompileShaderToSPIRV(const String& Source, const String& entryMethod, const ShaderStage stage, const ShaderLanguage language, uint32** ppBytesOut, uint32& bytesSizeOut, String& errorMessageOut)
	{
		shaderc::Compiler vkCompiler;
		shaderc::CompileOptions compileOptions;

		// Set the source language
		compileOptions.SetSourceLanguage(ShaderCompilerUtils::GetLanguage(language));
		compileOptions.SetSuppressWarnings();

		// Preprocess the shader
		shaderc::PreprocessedSourceCompilationResult preprocessedResult = vkCompiler.PreprocessGlsl(Source, 
			ShaderCompilerUtils::GetShaderKind(stage), entryMethod.c_str(), compileOptions);

		// Check for errors
		if(preprocessedResult.GetCompilationStatus() != shaderc_compilation_status_success || preprocessedResult.GetNumErrors() > 0)
		{
			errorMessageOut = preprocessedResult.GetErrorMessage();
			ppBytesOut = nullptr;
			bytesSizeOut = 0;
			return false;
		}

		// Compile the shader
		shaderc::SpvCompilationResult spirvResult = vkCompiler.CompileGlslToSpv(Source, 
			ShaderCompilerUtils::GetShaderKind(stage), entryMethod.c_str(), compileOptions);

		// Check for errors
		if(spirvResult.GetCompilationStatus() != shaderc_compilation_status_success || spirvResult.GetNumErrors() > 0)
		{
			errorMessageOut = spirvResult.GetErrorMessage();
			ppBytesOut = nullptr;
			bytesSizeOut = 0;
			return false;
		}

		// Copy the SPIRV data
		const uint64 bufferSize = (byte*)spirvResult.cend() - (byte*)spirvResult.cbegin();
		uint32* pBuffer = new uint32[bufferSize];
		memcpy(pBuffer, spirvResult.cbegin(), bufferSize);
		*ppBytesOut = pBuffer;
		bytesSizeOut = bufferSize;

		return true;
	}

	String ShaderCompiler::GetShaderCodeAsString(const String& filePath)
	{
		std::ifstream file(filePath, std::ios::binary);

		if (!file)
		{
			CORE_LOG(HE_WARNING, "ShaderCompiler", "Failed to read ShaderFile");
			return "";
		}

		file.seekg(0, std::ios::end);
		std::streamsize fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		String buffer(fileSize, '\0');
		if (!file.read(buffer.data(), fileSize))
		{
			CORE_LOG(HE_WARNING, "ShaderCompiler", "Failed to read ShaderFile");
			return "";
		}

		return buffer;

	}
}