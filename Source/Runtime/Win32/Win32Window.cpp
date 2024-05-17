#include "Win32Window.h"

namespace Hollow
{

#if defined(HOLLOW_PLATFORM_WINDOWS)
	Win32Window* GetWindowUserData(const HWND hwnd)
	{
		return reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, -21));
	}
#endif

	Win32Window::Win32Window(const WindowDesc& desc) : PlatformWindow(desc)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		mHandle = nullptr;
		mInstance = GetModuleHandle(nullptr);
		mContext = nullptr;

		const char className[] = "HollowWin32WindowClass";
		WNDCLASSEX windowClass = {};
		windowClass.cbSize = sizeof(windowClass);
		windowClass.hInstance = mInstance;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = sizeof(Win32Window*);
		windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = className;
		windowClass.lpfnWndProc = WndProc;

		CORE_ASSERT(RegisterClassEx(&windowClass) != NULL, "Win32Window", "Failed to register Win32 Window.");

		mHandle = CreateWindowEx(0, className, desc.WindowTitle.c_str(), WS_OVERLAPPEDWINDOW | WS_EX_ACCEPTFILES,
			desc.WindowPosition.x, desc.WindowPosition.y, desc.WindowSize.x , desc.WindowSize.y,
			nullptr, nullptr, windowClass.hInstance, this);

		CORE_ASSERT(mHandle != nullptr, "Win32Window", "Failed to create Win32 Window.");

		if (mHandle)
		{
			switch (desc.WindowMode)
			{
			case WindowMode::Windowed:
			{
				// Find the center of the monitor
				Vector2i monitorCenter = { (desc.pMonitor->GetMonitorResolution().x - (int32)desc.WindowSize.x) / 2 , (desc.pMonitor->GetMonitorResolution().y - (int32)desc.WindowSize.y) / 2 };
				SetWindowPosInternal(monitorCenter);
				SetWindowLong(mHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW);
				SetWindowPos(mHandle, HWND_TOP, monitorCenter.x, monitorCenter.y, desc.WindowSize.x, desc.WindowSize.y, SWP_FRAMECHANGED);
				break;
			}
			case WindowMode::Borderless:
				SetWindowLong(mHandle, GWL_STYLE, WS_POPUP);
				SetWindowSizeInternal({ static_cast<uint32>(desc.pMonitor->GetMonitorResolution().x), static_cast<uint32>(desc.pMonitor->GetMonitorResolution().y) });
				SetWindowPos(mHandle, HWND_TOP, 0, 0, desc.pMonitor->GetMonitorResolution().x, desc.pMonitor->GetMonitorResolution().y, SWP_NOMOVE | SWP_FRAMECHANGED);
				break;
			case WindowMode::Fullscreen:
				SetWindowLong(mHandle, GWL_STYLE, WS_POPUP);
				SetWindowSizeInternal({ static_cast<uint32>(desc.pMonitor->GetMonitorResolution().x), static_cast<uint32>(desc.pMonitor->GetMonitorResolution().y) });
				SetWindowPos(mHandle, HWND_TOP, 0, 0, desc.pMonitor->GetMonitorResolution().x, desc.pMonitor->GetMonitorResolution().y, SWP_NOMOVE | SWP_FRAMECHANGED);
				break;
			}
		}

		if (mHandle)
			mContext = GetDC(mHandle);
#endif
	}

	void Win32Window::SetWindowSizeImpl(Vector2u newSize)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		SetWindowPos(mHandle, HWND_TOP, 0, 0, newSize.x, newSize.y, SWP_NOMOVE | SWP_FRAMECHANGED);
#endif
	}
	void Win32Window::SetWindowTitleImpl(const String& newTitle)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		SetWindowText(mHandle, newTitle.c_str());
#endif
	}

	void Win32Window::SetWindowModeImpl(WindowMode newMode)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		switch (newMode)
		{
		case WindowMode::Windowed:
			SetWindowLong(mHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			break;
		case WindowMode::Borderless:
			SetWindowLong(mHandle, GWL_STYLE, WS_POPUP | WS_OVERLAPPEDWINDOW);
			break;
		case WindowMode::Fullscreen:
			SetWindowLong(mHandle, GWL_STYLE, WS_POPUP);
			break;
		}
#endif
	}

	void Win32Window::SetWindowPositionImpl(Vector2i newPosition)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		SetWindowPos(mHandle, HWND_TOP, newPosition.x, newPosition.y, 0, 0, SWP_NOSIZE | SWP_FRAMECHANGED);
#endif
	}

	void Win32Window::ShowImpl()
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		ShowWindow(mHandle, SW_SHOW);
#endif
	}

	void Win32Window::HideImpl()
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		ShowWindow(mHandle, SW_HIDE);
#endif

	}

	void Win32Window::PollEventsImpl()
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		MSG msg;
		while (PeekMessage(&msg, mHandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
#endif
	}

	void Win32Window::OnShutdown()
	{
	}

#if defined(HOLLOW_PLATFORM_WINDOWS)
	LRESULT Win32Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CREATE:
		{
			Win32Window* window = (Win32Window*)((LPCREATESTRUCT)lParam)->lpCreateParams;
			SetWindowLongPtr(hWnd, -21, (LONG_PTR)window);

			DragAcceptFiles(hWnd, TRUE);
			break;
		}
		case WM_CLOSE:
		{
			Win32Window* window = GetWindowUserData(hWnd);
			WindowEventDesc desc;
			desc.EventType = WindowEventType::WindowClosed;

			window->TriggerWindowEvent(desc);
			break;
		}
		case WM_SIZE:
		{
			Win32Window* window = GetWindowUserData(hWnd);
			WindowEventDesc desc;
			desc.EventType = WindowEventType::WindowResized;
			desc.WindowSize.x = LOWORD(lParam);
			desc.WindowSize.y = HIWORD(lParam);

			window->TriggerWindowEvent(desc);
			break;
		}
		case WM_MOVE:
		{
			Win32Window* window = GetWindowUserData(hWnd);
			WindowEventDesc desc;
			desc.EventType = WindowEventType::WindowMoved;
			desc.WindowPosition.x = LOWORD(lParam);
			desc.WindowPosition.y = HIWORD(lParam);

			window->TriggerWindowEvent(desc);
			break;
		}
		case WM_DROPFILES:
		{
			Win32Window* window = GetWindowUserData(hWnd);
			WindowEventDesc desc;
			desc.EventType = WindowEventType::FileDropped;

			HDROP hDrop = (HDROP)wParam;
			uint32 numFiles = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);
			for (uint32 i = 0; i < numFiles; i++)
			{
				char filePath[MAX_PATH];
				DragQueryFile(hDrop, i, filePath, MAX_PATH);
				desc.DroppedFiles.push_back(filePath);
			}

			DragFinish(hDrop);

			window->TriggerWindowEvent(desc);
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}
#endif
}
