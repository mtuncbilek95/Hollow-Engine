#include <Runtime/Platform/PlatformAPI.h>
#include <Runtime/Platform/PlatformFile.h>
#include <Runtime/Window/WindowManager.h>
#include <Runtime/Graphics/API/GraphicsManager.h>

#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/Fence/Fence.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/ShaderCompiler/ShaderCompiler.h>
#include <Runtime/Graphics/Pipeline/Pipeline.h>
#include <Runtime/Graphics/Sampler/Sampler.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Texture/TextureBuffer.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayout.h>
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Resource/Importer/ResourceImporter.h>

#include <Runtime/Resource/Texture/TextureResource.h>
#include <Runtime/Resource/Mesh/MeshResource.h>

#define CO_FACTOR .2f
#define MSAA_SAMPLES 8

using namespace Hollow;

i32 main(i32 argC, char** argV)
{
	PlatformAPI::GetAPI().InitializeArguments(argC, argV);

#pragma region Window Creation
	// Create a window
	Hollow::WindowDesc desc = {};
	desc.WindowSize = { 1300, 1300 };
	desc.WindowPosition = { 0, 0 };
	desc.WindowTitle = "Hollow - PBR";
	desc.WindowMode = WindowMode::Windowed;

	auto mWindow = WindowManager::GetAPI().InitializeWindow(desc);
#pragma endregion

#pragma region Graphics Creation
	// Create a graphics instance
	GraphicsInstanceDesc instanceDesc = {};
	instanceDesc.API = GraphicsAPI::Vulkan;
	instanceDesc.ApplicationName = "Hollow Engine";
	instanceDesc.InstanceName = "Hollow Engine";
	auto mGraphicsInstance = GraphicsInstance::CreateInstance(instanceDesc);

	// Create a graphics device
	GraphicsDeviceDesc deviceDesc = {};
	deviceDesc.Instance = mGraphicsInstance;
	deviceDesc.GraphicsQueueCount = 1;
	deviceDesc.ComputeQueueCount = 1;
	deviceDesc.TransferQueueCount = 1;
	auto mDevice = mGraphicsInstance->CreateGraphicsDevice(deviceDesc);

	// Create a swapchain
	SwapchainDesc swapchainDesc = {};
	swapchainDesc.BufferCount = 2;
	swapchainDesc.ImageSize = mWindow->GetWindowResolution();
	swapchainDesc.SwapchainImageFormat = TextureFormat::RGBA8_UNorm;
	swapchainDesc.SwapchainUsage = TextureUsage::ColorAttachment;
	swapchainDesc.VSync = PresentMode::FullVSync;
	swapchainDesc.SwapchainMode = ShareMode::Exclusive;
	swapchainDesc.pQueue = GraphicsManager::GetAPI().GetPresentQueue();

	auto mSwapchain = mDevice->CreateSwapchain(swapchainDesc);

	GraphicsManager::GetAPI().SetMemoryBudget(500, 500);
#pragma endregion

#pragma region Shader Creation
	MemoryBuffer vBuffer = {};
	MemoryOwnedBuffer vShaderCode;
	String errorMessage;
	PlatformFile::Read(PlatformAPI::GetAPI().GetEngineSourcePath() + "Tests/02_PBR/Shaders/PBRVertex.vert", vBuffer);
	ShaderDesc vertexShaderDesc = {};

	if (ShaderCompiler::CompileShaderToSPIRV(vBuffer, "main", ShaderStage::Vertex, ShaderLanguage::GLSL, vShaderCode, errorMessage))
	{
		vertexShaderDesc.ShaderName = "PBRVertex";
		vertexShaderDesc.Stage = ShaderStage::Vertex;
		vertexShaderDesc.EntryPoint = "main";
		vertexShaderDesc.Language = ShaderLanguage::GLSL;
		vertexShaderDesc.ShaderCode = vShaderCode;
	}

	auto mVertexShader = mDevice->CreateShader(vertexShaderDesc);

	MemoryBuffer fBuffer = {};
	MemoryOwnedBuffer fShaderCode;
	PlatformFile::Read(PlatformAPI::GetAPI().GetEngineSourcePath() + "Tests/02_PBR/Shaders/PBRFrag.frag", fBuffer);
	ShaderDesc fragmentShaderDesc = {};

	if (ShaderCompiler::CompileShaderToSPIRV(fBuffer, "main", ShaderStage::Fragment, ShaderLanguage::GLSL, fShaderCode, errorMessage))
	{
		fragmentShaderDesc.ShaderName = "PBRFragment";
		fragmentShaderDesc.Stage = ShaderStage::Fragment;
		fragmentShaderDesc.EntryPoint = "main";
		fragmentShaderDesc.Language = ShaderLanguage::GLSL;
		fragmentShaderDesc.ShaderCode = fShaderCode;
	}

	auto mFragmentShader = mDevice->CreateShader(fragmentShaderDesc);
#pragma endregion

#pragma region Fence Creation
	DArray<SharedPtr<Fence>> mRuntimeFences;
	for (byte i = 0; i < swapchainDesc.BufferCount; i++)
		mRuntimeFences.push_back(mDevice->CreateSyncFence({ false }));

	auto mCompileFence = mDevice->CreateSyncFence({ false });
#pragma endregion

#pragma region Command Creation
	CommandPoolDesc commandPoolDesc = {};
	commandPoolDesc.PoolType = CommandPoolType::Graphics;

	auto mCommandPool = mDevice->CreateCommandPool(commandPoolDesc);

	CommandBufferDesc commandBufferDesc = {};
	commandBufferDesc.pOwnerPool = mCommandPool;

	auto mCompileCmd = mDevice->CreateCommandBuffer(commandBufferDesc);

	DArray<SharedPtr<CommandBuffer>> mRuntimeCmd;
	for (byte i = 0; i < swapchainDesc.BufferCount; i++)
		mRuntimeCmd.push_back(mDevice->CreateCommandBuffer(commandBufferDesc));
#pragma endregion
}
