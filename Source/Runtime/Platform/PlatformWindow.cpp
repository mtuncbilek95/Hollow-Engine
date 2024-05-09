#include "PlatformWindow.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Runtime/Win32/Win32Window.h>
#endif

namespace Hollow
{
	sharedPtr<PlatformWindow> PlatformWindow::InitializeWindow(const WindowDesc& desc)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		return std::make_shared<Win32Window>(desc);
#endif
	}
	PlatformWindow::PlatformWindow(const WindowDesc& desc) : Object(), mWindowSize(desc.WindowSize),
		mWindowPosition(desc.WindowPosition), mWindowTitle(desc.WindowTitle), mWindowMode(desc.WindowMode)
	{
	}

	void PlatformWindow::PollEvents()
	{
		mEventQueue.clear();
	}

	void PlatformWindow::TriggerWindowEvent(const WindowEventDesc& desc)
	{
		switch (desc.EventType)
		{
		case WindowEventType::None:
		{
			break;
		}
		case WindowEventType::WindowClosed:
		{
			// Close the window
			break;
		}
		case WindowEventType::WindowResized:
		{
			SetWindowSize(desc.WindowSize);
			break;
		}
		case WindowEventType::WindowMoved:
		{
			SetWindowPosition(desc.WindowPosition);
			break;
		}
		case WindowEventType::WindowFocused:
		{
			// Focus the window
			break;
		}
		case WindowEventType::WindowLostFocus:
		{
			// Lose focus of the window
			break;
		}
		case WindowEventType::WindowMinimized:
		{
			// Minimize the window
			break;
		}
		case WindowEventType::WindowMaximized:
		{
			// Maximize the window
			break;
		}
		case WindowEventType::FileDropped:
		{
			// Drop a file
			break;
		}
		default:
			break;
		}

		mEventQueue.push_back(desc);
	}
}
