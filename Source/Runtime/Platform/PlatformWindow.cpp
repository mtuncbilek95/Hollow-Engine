#include "PlatformWindow.h"

#include <Runtime/Platform/PlatformMonitor.h>
#include <Runtime/Graphics/API/GraphicsManager.h>

namespace Hollow
{
	SharedPtr<PlatformWindow> PlatformWindow::InitializeWindow(const WindowDesc& desc)
	{
		return std::make_shared<PlatformWindow>(desc);
	}

	PlatformWindow::PlatformWindow(const WindowDesc& desc) : mWindowMode(desc.WindowMode), mWindowPosition({ 0,0 }), mWindowSize(desc.WindowSize),
		mWindowTitle(desc.WindowTitle), mVisible(true), mGLFWHandle(nullptr)
	{
		CORE_ASSERT(glfwInit(), "PlatformWindow", "Failed to initialize GLFW!");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		mGLFWHandle = glfwCreateWindow((i32)mWindowSize.x, (i32)mWindowSize.y, mWindowTitle.c_str(), nullptr, nullptr);

		CORE_ASSERT(mGLFWHandle, "PlatformWindow", "Failed to create GLFW window!");

		glfwSetWindowUserPointer(mGLFWHandle, this);

		Monitor primaryMonitor = PlatformMonitor::GetPrimaryMonitor();

		if (mWindowMode == WindowMode::Windowed)
		{
			glfwSetWindowAttrib(mGLFWHandle, GLFW_DECORATED, GLFW_TRUE);
			glfwSetWindowAttrib(mGLFWHandle, GLFW_RESIZABLE, GLFW_TRUE);
			i32 centerX = primaryMonitor.GetMonitorPosition().x + (i32)primaryMonitor.GetMonitorResolution().x / 2 - (i32)mWindowSize.x / 2;
			i32 centerY = primaryMonitor.GetMonitorPosition().y + (i32)primaryMonitor.GetMonitorResolution().y / 2 - (i32)mWindowSize.y / 2;

			SetWindowPosition({ centerX, centerY });
		}
		else
		{
			glfwSetWindowAttrib(mGLFWHandle, GLFW_DECORATED, GLFW_FALSE);
			glfwSetWindowAttrib(mGLFWHandle, GLFW_RESIZABLE, GLFW_FALSE);
			SetWindowPosition(primaryMonitor.GetMonitorPosition());
			SetWindowResolution(primaryMonitor.GetMonitorResolution());
		}

		glfwSetWindowPosCallback(mGLFWHandle, [](GLFWwindow* window, i32 x, i32 y)
			{
				PlatformWindow* platformWindow = static_cast<PlatformWindow*>(glfwGetWindowUserPointer(window));
				platformWindow->mWindowPosition = { x, y };
			});

		glfwSetWindowSizeCallback(mGLFWHandle, [](GLFWwindow* window, i32 width, i32 height)
			{
				PlatformWindow* platformWindow = static_cast<PlatformWindow*>(glfwGetWindowUserPointer(window));
				platformWindow->mWindowSize = { (u32)width, (u32)height };
				GraphicsManager::GetAPI().GetDefaultDevice()->GetSwapchain()->Resize(platformWindow->mWindowSize);
			});
	}

	PlatformWindow::~PlatformWindow()
	{
		glfwDestroyWindow(mGLFWHandle);
		glfwTerminate();
	}

	void PlatformWindow::Show()
	{
		glfwShowWindow(mGLFWHandle);
		mVisible = true;
	}

	void PlatformWindow::Hide()
	{
		glfwHideWindow(mGLFWHandle);
		mVisible = false;
	}

	void PlatformWindow::PollEvents()
	{
		glfwPollEvents();
	}

	void PlatformWindow::SetWindowResolution(const Vector2u& resolution)
	{
		glfwSetWindowSize(mGLFWHandle, (i32)resolution.x, (i32)resolution.y);
		mWindowSize = resolution;
	}

	void PlatformWindow::SetWindowPosition(const Vector2i& position)
	{
		glfwSetWindowPos(mGLFWHandle, position.x, position.y);
		mWindowPosition = position;
	}

	void PlatformWindow::SetWindowTitle(const String& title)
	{
		glfwSetWindowTitle(mGLFWHandle, title.c_str());
		mWindowTitle = title;
	}

	void PlatformWindow::SetWindowMode(const WindowMode& mode)
	{
		mWindowMode = mode;
	}

	void PlatformWindow::SetVisible(bool visible)
	{
		if (visible)
			Show();
		else
			Hide();
	}

}
