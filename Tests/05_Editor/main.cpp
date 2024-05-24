#include <Editor/ImGui/ImGuiRenderer.h>
#include <Runtime/Window/WindowManager.h>
#include <Runtime/Graphics/API/GraphicsManager.h>

#include <Runtime/Platform/PlatformMonitor.h>

using namespace Hollow;
int main()
{
	// Get the primary monitor
	auto primaryMonitor = PlatformMonitor::GetPrimaryMonitor();

	// Create a window
	Hollow::WindowDesc desc = {};
	desc.WindowSize = { 1920, 1080 };
	desc.WindowPosition = { 0, 0 };
	desc.WindowTitle = "Hollow Engine";
	desc.WindowMode = WindowMode::Windowed;
	desc.pMonitor = primaryMonitor;

	auto mWindow = WindowManager::GetInstanceAPI().InitializeWindow(desc);

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

	auto mImGuiRenderer = std::make_shared<ImGuiRenderer>();
	mImGuiRenderer->CreateImguiResources();

	mWindow->Show();
	while (mWindow->IsVisible())
	{
		mWindow->PollEvents();
		mImGuiRenderer->UpdateRender();
	}

	return 0;
}
