#include <Runtime/Platform/PlatformAPI.h>
#include <Runtime/Platform/PlatformFile.h>
#include <Runtime/ShaderCompiler/ShaderCompiler.h>

using namespace Hollow;

int main(int argC, char** argV)
{
	PlatformAPI::GetInstanceAPI().InitializeArguments(argC, argV);

	const String& engineSourcePath = PlatformAPI::GetInstanceAPI().GetEngineSourcePath();

	String shaderPath = engineSourcePath + "Tests/02_ShaderReflection/testVertex.vert";

	MemoryBuffer vBuffer = {};
	SharedPtr<MemoryOwnedBuffer> vShaderCode;
	String errorMessage;
	PlatformFile::Read(shaderPath, vBuffer);

	ShaderCompiler::CompileShaderToSPIRV(vBuffer, "main", ShaderStage::Vertex, ShaderLanguage::GLSL, vShaderCode, errorMessage);

	//ShaderCompiler::ReflectShader(vShaderCode);
}
