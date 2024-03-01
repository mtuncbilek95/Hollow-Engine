#include "Window.h"

namespace MiniVk
{

	Window::Window(const WindowDesc& desc) : mWindowHandle(nullptr), mWindowed(desc.Windowed),
		mWindowSize(desc.WindowSize), mWindowName(desc.WindowTitle), mWindowPosition(desc.WindowPosition)
	{
		DEV_ASSERT(glfwInit(), "Window", "Failed to initialize glfw");
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifndef MINIVK_DEBUG
		HWND console = GetConsoleWindow();
		ShowWindow(console, SW_HIDE);
#endif

		mWindowHandle = glfwCreateWindow(mWindowSize.x, mWindowSize.y, mWindowName.c_str(), nullptr, nullptr);
		glfwSetWindowPos(mWindowHandle, mWindowPosition.x, mWindowPosition.y);

		glfwShowWindow(mWindowHandle);

		DEV_ASSERT(mWindowHandle != nullptr, "Window", "Failed to create window");

		CORE_LOG(HE_VERBOSE, "PlatformWindow", "Operation is successful.");

		glfwSetWindowUserPointer(mWindowHandle, this);

		glfwSetWindowSizeCallback(mWindowHandle, [](GLFWwindow* window, int width, int height)
			{
				Window* win = (Window*)glfwGetWindowUserPointer(window);
				win->mWindowSize = { (uint32)width, (uint32)height };
			});

		glfwSetWindowPosCallback(mWindowHandle, [](GLFWwindow* window, int x, int y)
			{
				Window* win = (Window*)glfwGetWindowUserPointer(window);
				win->mWindowPosition = { x, y };
			});


	}

	Window::~Window()
	{
		glfwTerminate();
	}

	void Window::PollMessages()
	{
		glfwPollEvents();
	}
}
