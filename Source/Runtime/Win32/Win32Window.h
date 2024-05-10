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

	protected:
		virtual void SetWindowSizeImpl(Vector2u newSize) override;
		virtual void SetWindowTitleImpl(const String& newTitle) override;
		virtual void SetWindowModeImpl(WindowMode newMode) override;
		virtual void SetWindowPositionImpl(Vector2i newPosition) override;
		virtual void ShowImpl() override;
		virtual void HideImpl() override;

		virtual void PollEventsImpl() override;

		virtual void OnShutdown() override;

	private:

#if defined(HOLLOW_PLATFORM_WINDOWS)
		HWND mHandle;
		HINSTANCE mInstance;
		HDC mContext;

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif
	};
}
