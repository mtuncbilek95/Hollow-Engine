#include <Runtime/Platform/PlatformMonitor.h>
#include <Runtime/Platform/PlatformWindow.h>

int main()
{
	auto primaryMonitor = Hollow::PlatformMonitor::GetPrimaryMonitor();

	Hollow::WindowDesc desc;
	desc.WindowSize = { 800, 600 };
	desc.WindowPosition = { 0, 0 };
	desc.WindowTitle = "Hollow Engine";
	desc.WindowMode = Hollow::WindowMode::Windowed;

	auto window = Hollow::PlatformWindow::InitializeWindow(desc);

	window->Show();

	while (window->IsVisible())
	{
		window->PollEvents();
	}
	return 0;
}
