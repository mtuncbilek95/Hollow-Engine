#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifdef HOLLOW_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#include <Runtime/Window/WindowDesc.h>

namespace Hollow
{
	class RUNTIME_API Window
	{
	public:
		Window(const WindowDesc& desc);
		~Window();

		String GetTitle() const { return mWindowName; }
		Vector2u GetWindowSize() const { return mWindowSize; }
		Vector2i GetWindowPosition() const { return mWindowPosition; }

		GLFWwindow* GetWindowHandle() const { return mWindowHandle; }
		HWND GetWindowNativeHandle() const { return glfwGetWin32Window(mWindowHandle); }

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
