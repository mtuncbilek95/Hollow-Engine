#include <Runtime/Platform/PlatformAPI.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>

#include <Runtime/Resource/Shader/ShaderResource.h>


using namespace Hollow;

int main(int argC, char** argV)
{
	PlatformAPI::GetInstanceAPI().InitializeArguments(argC, argV);

	const String& engineSourcePath = PlatformAPI::GetInstanceAPI().GetEngineSourcePath();
	String shaderPath = engineSourcePath + "Tests/02_ShaderReflection/testVertex.vert";

	GraphicsInstanceDesc instanceDesc = {};
	instanceDesc.ApplicationName = "Shader Reflection Test";
	instanceDesc.API = GraphicsAPI::Vulkan;
	instanceDesc.InstanceName = "Shader Reflection Test Instance";

	auto mInstance = GraphicsInstance::CreateInstance(instanceDesc);

	GraphicsDeviceDesc deviceDesc = {};
	deviceDesc.Instance = mInstance;
	deviceDesc.ComputeQueueCount = 1;
	deviceDesc.GraphicsQueueCount = 1;
	deviceDesc.TransferQueueCount = 1;

	auto mDevice = mInstance->CreateGraphicsDevice(deviceDesc);

	ShaderResource shaderResource;
	shaderResource.CompileShader(shaderPath, "TestVertex", "main", ShaderStage::Vertex, ShaderLanguage::GLSL);

	CORE_LOG(HE_INFO, "Shader Reflection Test", "Reflection Output Count: %d", shaderResource.GetReflection()->GetOutputDescriptors().size());
	return 0;
}
