#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifdef MINIVK_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#include <Window/WindowDesc.h>

namespace MiniVk
{
	class Window
	{
	public:
		Window(const WindowDesc& desc);
		~Window();

		const String& GetTitle() const { return mWindowName; }
		const Vector2u& GetWindowSize() const { return mWindowSize; }
		const Vector2i& GetWindowPosition() const { return mWindowPosition; }

		GLFWwindow* GetWindowHandle() const { return mWindowHandle; }

		// If the platform is Windows, be able to get the native handle.
#ifdef MINIVK_PLATFORM_WINDOWS 
		HWND GetWindowNativeHandle() const { return glfwGetWin32Window(mWindowHandle); }
#endif

		bool ShouldClose() const { return glfwWindowShouldClose(mWindowHandle); }
		bool GetWindowed() const { return mWindowed; }

		void PollMessages();

	private:
		GLFWwindow* mWindowHandle;
		Vector2u mWindowSize;
		Vector2i mWindowPosition;
		String mWindowName;
		bool mWindowed;
	};
}
