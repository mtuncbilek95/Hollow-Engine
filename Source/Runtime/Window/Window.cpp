#include "Window.h"

//#include <Runtime/Resource/TextureLoader/TextureLoader.h>

namespace Hollow
{

	Window::Window(const WindowDesc& desc) : mWindowHandle(nullptr), mWindowed(desc.Windowed),
		mWindowSize(desc.WindowSize), mWindowName(desc.WindowTitle), mWindowPosition(desc.WindowPosition)
	{
		DEV_ASSERT(glfwInit(), "Window", "Failed to initialize glfw");
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
		glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);

		mWindowHandle = glfwCreateWindow(mWindowSize.x, mWindowSize.y, mWindowName.c_str(), nullptr, nullptr);
		glfwSetWindowPos(mWindowHandle, mWindowPosition.x, mWindowPosition.y);

		glfwShowWindow(mWindowHandle);

		glfwMakeContextCurrent(mWindowHandle);

		DEV_ASSERT(mWindowHandle != nullptr, "Window", "Failed to create window");

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

	void Window::PollMessages()
	{
		glfwSwapBuffers(mWindowHandle);
		glfwPollEvents();
	}

	void Window::OnShutdown() noexcept
	{
		glfwDestroyWindow(mWindowHandle);
		glfwTerminate();

		CORE_LOG(HE_VERBOSE, "Window", "Window destroyed");
	}
}
