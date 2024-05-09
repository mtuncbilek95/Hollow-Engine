#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Platform/PlatformWindow.h>

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

namespace Hollow
{
	class RUNTIME_API Win32Window : public PlatformWindow
	{
	public:
		Win32Window(const WindowDesc& desc);
		~Win32Window() override = default;

#if defined(HOLLOW_PLATFORM_WINDOWS)
		HWND GetHandle() const { return mHandle; }
		HINSTANCE GetInstance() const { return mInstance; }
#endif

		virtual void SetWindowSize(Vector2u newSize) override;
		virtual void SetWindowTitle(const string& newTitle) override;
		virtual void SetWindowMode(WindowMode newMode) override;
		virtual void SetWindowPosition(Vector2i newPosition) override;
		virtual void Show() override;
		virtual void Hide() override;

		virtual void PollEvents() override;

	private:

#if defined(HOLLOW_PLATFORM_WINDOWS)
		HWND mHandle;
		HINSTANCE mInstance;
		HDC mContext;

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif
	};
}
