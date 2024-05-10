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

	void PlatformWindow::SetWindowSize(Vector2u newSize)
	{
		SetWindowSizeImpl(newSize);
		mWindowSize = newSize;
	}

	void PlatformWindow::SetWindowTitle(const string& newTitle)
	{
		SetWindowTitleImpl(newTitle);
		mWindowTitle = newTitle;
	}

	void PlatformWindow::SetWindowMode(WindowMode newMode)
	{
		SetWindowModeImpl(newMode);
		mWindowMode = newMode;
	}

	void PlatformWindow::SetWindowPosition(Vector2i newPosition)
	{
		SetWindowPositionImpl(newPosition);
		mWindowPosition = newPosition;
	}

	void PlatformWindow::Show()
	{
		ShowImpl();
		mVisible = true;
	}

	void PlatformWindow::Hide()
	{
		HideImpl();
		mVisible = false;
	}

	void PlatformWindow::PollEvents()
	{
		mEventQueue.clear();
		PollEventsImpl();
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
			OnWindowClose();
			// Close the window
			break;
		}
		case WindowEventType::WindowResized:
		{
			OnWindowResize(desc.WindowSize);
			break;
		}
		case WindowEventType::WindowMoved:
		{
			OnWindowMove(desc.WindowPosition);
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

	void PlatformWindow::OnWindowClose()
	{
		mVisible = false;
	}

	void PlatformWindow::OnWindowResize(Vector2u newSize)
	{
		// Check if window is minimized or maximized
		// Resize the swapchain according to everything
	}

	void PlatformWindow::OnWindowMove(Vector2i newPosition)
	{
		// Check if the window is switching between monitors
		// If it is, update the monitor and the window position
	}
}
