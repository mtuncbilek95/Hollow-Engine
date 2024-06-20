#include "ShaderResource.h"

#include <Runtime/Graphics/API/GraphicsManager.h>
#include <Runtime/Platform/PlatformAPI.h>
#include <Runtime/Platform/PlatformFile.h>
#include <Runtime/ShaderCompiler/ShaderCompiler.h>

namespace Hollow
{
	ShaderResource::ShaderResource() : ResourceSubObject()
	{
		mGraphicsDevice = GraphicsManager::GetAPI().GetDefaultDevice();
	}

	void ShaderResource::CompileShader(const String path, const String shaderName, const String EntryPoint, const ShaderStage stage, const ShaderLanguage lang)
	{
		//MemoryBuffer shaderBuffer = {};
		//MemoryOwnedBuffer shaderCode;
		//String errorMessage;

		//CORE_ASSERT(PlatformFile::Read(path, shaderBuffer),
		//	"ShaderResource", "Failed to read shader file");

		//CORE_ASSERT(ShaderCompiler::CompileShaderToSPIRV(shaderBuffer, EntryPoint, stage, lang,
		//	shaderCode, errorMessage), "ShaderResource", "Failed to compile shader to SPIRV");

		//ShaderDesc vertexShaderDesc = {};
		//vertexShaderDesc.ShaderName = shaderName;
		//vertexShaderDesc.Stage = stage;
		//vertexShaderDesc.EntryPoint = EntryPoint;
		//vertexShaderDesc.Language = lang;
		//vertexShaderDesc.ShaderCode = shaderCode;

		//mShaderModule = mGraphicsDevice->CreateShader(vertexShaderDesc);

		//CORE_ASSERT(ShaderCompiler::ReflectShader(shaderCode, mReflection), "ShaderResource", "Failed to reflect shader");
	}
}
