#include <Engine/Window/WindowManager.h>
#include <Engine/Graphics/API/GraphicsAPI.h>
#include <Editor/ImguiRenderer/ImguiRenderer.h>

#include <Engine/Graphics/Device/GraphicsDevice.h>
#include <Engine/Graphics/Swapchain/Swapchain.h>


using namespace Hollow;

int main()
{
	WindowDesc desc;
	desc.WindowTitle = "Test";
	desc.WindowSize = Vec2u(800, 600);
	desc.WindowMode = WindowMode::Windowed;
	desc.WindowPosition = Vec2i(100, 100);

	auto window = WindowAPI::GetAPI()->InitializeWindow(desc);

	GraphicsInstanceDesc instanceDesc = {};
	instanceDesc.AppName = "Test";
	instanceDesc.EngineName = "Hollow";
	instanceDesc.AppVersion = { 1, 0, 0 };
	instanceDesc.EngineVersion = { 1, 0, 0 };

	auto instance = GraphicsAPI::GetAPI()->CreateInstance(instanceDesc);

	auto device = GraphicsAPI::GetAPI()->CreateDevice();

	SwapchainDesc swapchainDesc = {};
	swapchainDesc.BufferCount = 2;
	swapchainDesc.ImageFormat = TextureFormat::RGBA8_UNorm;
	swapchainDesc.pRequestQueue = GraphicsAPI::GetAPI()->GetGraphicsQueue();
	swapchainDesc.TextureUsage = TextureUsageFlags::ColorAttachment;
	swapchainDesc.VSync = PresentMode::FullVSync;
	swapchainDesc.ImageSize = desc.WindowSize;

	auto swapchain = GraphicsAPI::GetAPI()->CreateSwapchain(swapchainDesc);

	auto imguiRenderer = std::make_shared<ImguiRenderer>();

	auto fence1 = device.lock()->CreateGraphicsFence(false);
	auto fence2 = device.lock()->CreateGraphicsFence(false);

	window->Show();

	while (!window->IsClosed())
	{
		window->PollEvents();
	
		u32 index = swapchain.lock()->AcquireNextImage(fence1, nullptr);

		device.lock()->WaitFence(fence1);
		device.lock()->ResetFence(fence1);

		imguiRenderer->BeginFrame();
		imguiRenderer->EndFrame();

		swapchain.lock()->Present(nullptr, index);
	}

	window->Hide();

	return 0;
}
