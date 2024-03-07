#include <Runtime/Window/WindowManager.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/Semaphore/Semaphore.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/ShaderCompiler/ShaderCompiler.h>

using namespace Hollow;

int main()
{
	// Create a window
	WindowDesc winDesc = {};
	winDesc.Windowed = true;
	winDesc.WindowSize = { 1280, 720 };
	winDesc.WindowTitle = "Test Application";
	winDesc.WindowPosition = { 100, 100 };

	auto mWindow = WindowManager::GetInstance().CreateAppWindow(winDesc);

	// Create a graphics instance
	GraphicsInstanceDesc instanceDesc = {};
	instanceDesc.API = GraphicsAPI::Vulkan;
	instanceDesc.InstanceName = "Hollow Engine";
	instanceDesc.ApplicationName = "Test Application";
	instanceDesc.EnabledExtensions = {};

	auto mGraphicsInstance = GraphicsInstance::CreateInstance(instanceDesc);
	mGraphicsInstance->EnumerateAdapters();

	// Create a graphics device
	auto mDevice = mGraphicsInstance->CreateDevice();

	// Create a present queue for swapchain
	GraphicsQueueDesc presentQueueDesc = {};
	presentQueueDesc.QueueType = GraphicsQueueType::Graphics;

	auto mPresentQueue = mDevice->CreateGraphicsQueue(presentQueueDesc);

	// Create swapchain for the window
	SwapchainDesc swapchainDesc = {};
	swapchainDesc.BufferCount = 2;
	swapchainDesc.ImageSize = winDesc.WindowSize;
	swapchainDesc.pQueue = mPresentQueue.get();
	swapchainDesc.SwapchainImageFormat = TextureFormat::RGBA8_UNorm;
	swapchainDesc.SwapchainUsage = TextureUsage::ColorAttachment;
	swapchainDesc.VSync = PresentMode::FullVSync;
	swapchainDesc.SwapchainMode = ShareMode::Exclusive;

	auto mSwapchain = mDevice->CreateSwapchain(swapchainDesc);

	// Create present semaphore for each frame
	Array<SharedPtr<Semaphore>> presentSemaphores(swapchainDesc.BufferCount);
	for (byte i = 0; i < swapchainDesc.BufferCount; i++)
	{
		presentSemaphores[i] = mDevice->CreateSyncSemaphore();
	}

	// Create image semaphore for each frame
	Array<SharedPtr<Semaphore>> imageSemaphores(swapchainDesc.BufferCount);
	for (byte i = 0; i < swapchainDesc.BufferCount; i++)
	{
		imageSemaphores[i] = mDevice->CreateSyncSemaphore();
	}

	// Create vertex shader
	ShaderDesc vsDesc = {};
	vsDesc.EntryPoint = "main";
	vsDesc.Language = ShaderLanguage::GLSL;
	vsDesc.ShaderName = "Test Vertex Shader";
	vsDesc.Source = ShaderCompiler::GetShaderCodeAsString(R"(D:\Projects\Another\Hollow-Engine\Shaders\testVertex.vert)");
	vsDesc.Stage = ShaderStage::Vertex;
	
	auto mVertexShader = mDevice->CreateShader(vsDesc);

	// Create fragment shader
	ShaderDesc fsDesc = {};
	fsDesc.EntryPoint = "main";
	fsDesc.Language = ShaderLanguage::GLSL;
	fsDesc.ShaderName = "Test Fragment Shader";
	fsDesc.Source = ShaderCompiler::GetShaderCodeAsString(R"(D:\Projects\Another\Hollow-Engine\Shaders\testFragment.frag)");
	fsDesc.Stage = ShaderStage::Fragment;

	auto mFragmentShader = mDevice->CreateShader(fsDesc);

	
	while (!mWindow->ShouldClose())
	{
		Semaphore* presentSemaphore = presentSemaphores[mSwapchain->GetCurrentFrameIndex()].get();
		mWindow->PollMessages();
		mSwapchain->Present(&presentSemaphore, 1);
	}

	return 0;
}