#include <Runtime/Platform/PlatformMonitor.h>
#include <Runtime/Window/WindowManager.h>


#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>

using namespace Hollow;

int main()
{
	auto primaryMonitor = PlatformMonitor::GetPrimaryMonitor();

	Hollow::WindowDesc desc = {};
	desc.WindowSize = { 800, 600 };
	desc.WindowPosition = { 0, 0 };
	desc.WindowTitle = "Hollow Engine";
	desc.WindowMode = WindowMode::Windowed;

	auto window = WindowManager::GetInstanceAPI().InitializeWindow(desc);
	window->Show();

	GraphicsInstanceDesc instanceDesc = {};
	instanceDesc.API = GraphicsAPI::Vulkan;
	instanceDesc.ApplicationName = "Hollow Engine";
	instanceDesc.InstanceName = "Hollow Engine";
	instanceDesc.EnabledExtensions = {};
	auto instance = GraphicsInstance::CreateInstance(instanceDesc);

	GraphicsDeviceDesc deviceDesc = {};
	deviceDesc.Instance = instance;
	deviceDesc.GraphicsQueueCount = 1;
	deviceDesc.ComputeQueueCount = 1;
	deviceDesc.TransferQueueCount = 1;
	auto device = instance->CreateGraphicsDevice(deviceDesc);

	GraphicsQueueDesc queueDesc = {};
	queueDesc.QueueType = GraphicsQueueType::Graphics;
	
	auto presentQueue = device->CreateQueue(queueDesc);

	SwapchainDesc swapchainDesc = {};
	swapchainDesc.BufferCount = 2;
	swapchainDesc.ImageSize = { 800, 600 };
	swapchainDesc.SwapchainImageFormat = TextureFormat::RGBA8_UNorm;
	swapchainDesc.SwapchainUsage = TextureUsage::ColorAttachment;
	swapchainDesc.VSync = PresentMode::FullVSync;
	swapchainDesc.SwapchainMode = ShareMode::Exclusive;
	swapchainDesc.pQueue = presentQueue;

	auto swapchain = device->CreateSwapchain(swapchainDesc);

	while (window->IsVisible())
	{
		window->PollEvents();
	}

	return 0;
}
