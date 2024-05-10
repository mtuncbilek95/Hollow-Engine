#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>
#include <Runtime/Object/Object.h>
#include <Runtime/Window/WindowDesc.h>
#include <Runtime/Window/WindowEventDesc.h>

namespace Hollow
{
	class RUNTIME_API Swapchain;

	class RUNTIME_API PlatformWindow : public Object
	{
		friend class GraphicsDevice;
		friend class WindowManager;
	public:
		static SharedPtr<PlatformWindow> InitializeWindow(const WindowDesc& desc);

	public:
		PlatformWindow(const WindowDesc& desc);
		virtual ~PlatformWindow() override = default;

		const Vector2u& GetWindowsize() const { return mWindowSize; }
		const Vector2i& GetWindowPosition() const { return mWindowPosition; }
		const String& GetWindowTitle() const { return mWindowTitle; }
		const WindowMode& GetWindowMode() const { return mWindowMode; }
		const bool IsVisible() const { return mVisible; }

		void SetWindowSize(Vector2u newSize);
		void SetWindowTitle(const String& newTitle);
		void SetWindowMode(WindowMode newMode);
		void SetWindowPosition(Vector2i newPosition);

		void Show();
		void Hide();

		void PollEvents();

		virtual void OnShutdown() override;

	protected:
		virtual void SetWindowSizeImpl(Vector2u newSize) = 0;
		virtual void SetWindowTitleImpl(const String& newTitle)  = 0;
		virtual void SetWindowModeImpl(WindowMode newMode) = 0;
		virtual void SetWindowPositionImpl(Vector2i newPosition) = 0;

		virtual void ShowImpl() = 0;
		virtual void HideImpl() = 0;

		virtual void PollEventsImpl() = 0;

	protected:
		void TriggerWindowEvent(const WindowEventDesc& desc);

		void OnWindowClose();
		void OnWindowResize(Vector2u newSize);
		void OnWindowMove(Vector2i newPosition);

	private:
		void SetConnectedSwapchain(SharedPtr<Swapchain> swapchain) { mConnectedSwapchain = swapchain; }

	private:
		Vector2u mWindowSize;
		Vector2i mWindowPosition;
		String mWindowTitle;
		WindowMode mWindowMode;
		bool mVisible;

		SharedPtr<Swapchain> mConnectedSwapchain;

		ArrayList<WindowEventDesc> mEventQueue;
	};
}
