#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>
#include <Runtime/Object/Object.h>
#include <Runtime/Window/WindowDesc.h>

#include <Runtime/Graphics/Swapchain/Swapchain.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if defined(HOLLOW_PLATFORM_WINDOWS)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

namespace Hollow
{
	class RUNTIME_API PlatformWindow : public Object
	{
		friend class GraphicsDevice;
		friend class WindowManager;
	public:
		static SharedPtr<PlatformWindow> InitializeWindow(const WindowDesc& desc);

	public:
		PlatformWindow(const WindowDesc& desc);
		virtual ~PlatformWindow() override;

		void Show();
		void Hide();

		void PollEvents();

		const Vector2u& GetWindowResolution() const { return mWindowSize; }
		const Vector2i& GetWindowPosition() const { return mWindowPosition; }
		const String& GetWindowTitle() const { return mWindowTitle; }
		const WindowMode& GetWindowMode() const { return mWindowMode; }
		const bool IsVisible() const { return mVisible; }
		const bool IsMinimized() const { return glfwGetWindowAttrib(mGLFWHandle, GLFW_ICONIFIED); }
		const bool IsMaximized() const { return glfwGetWindowAttrib(mGLFWHandle, GLFW_MAXIMIZED); }
		const bool IsClosed() const { return glfwWindowShouldClose(mGLFWHandle); }

#if defined(HOLLOW_PLATFORM_WINDOWS)
		HWND GetWindowHandle() const { return glfwGetWin32Window(mGLFWHandle); }
		HINSTANCE GetInstanceHandle() const { return GetModuleHandle(NULL); }
#endif

		GLFWwindow* GetGLFWHandle() const { return mGLFWHandle; }

		void SetWindowResolution(const Vector2u& resolution);
		void SetWindowPosition(const Vector2i& position);
		void SetWindowTitle(const String& title);
		void SetWindowMode(const WindowMode& mode);
		void SetVisible(bool visible);

	private:
		Vector2u mWindowSize;
		Vector2i mWindowPosition;
		String mWindowTitle;
		WindowMode mWindowMode;
		bool mVisible;

		GLFWwindow* mGLFWHandle;
	};
}
