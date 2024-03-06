#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Window/WindowManager.h>

using namespace Hollow;

int main()
{
	WindowDesc winDesc = {};
	winDesc.Windowed = true;
	winDesc.WindowSize = { 1280, 720 };
	winDesc.WindowTitle = "Test Application";
	winDesc.WindowPosition = { 100, 100 };

	auto mWindow = WindowManager::GetInstance().CreateAppWindow(winDesc);

	GraphicsInstanceDesc instanceDesc = {};
	instanceDesc.API = GraphicsAPI::Vulkan;
	instanceDesc.InstanceName = "Hollow Engine";
	instanceDesc.ApplicationName = "Test Application";
	instanceDesc.EnabledExtensions = {};

	auto mGraphicsInstance = GraphicsInstance::CreateInstance(instanceDesc);
	mGraphicsInstance->EnumerateAdapters();

	auto mDevice = mGraphicsInstance->CreateDevice();

	while (!mWindow->ShouldClose())
	{
		mWindow->PollMessages();
	}

	return 0;
}