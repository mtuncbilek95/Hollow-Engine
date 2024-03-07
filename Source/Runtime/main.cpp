#include <Runtime/Window/WindowManager.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>

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
	
	while (!mWindow->ShouldClose())
	{
		mWindow->PollMessages();
	}

	return 0;
}