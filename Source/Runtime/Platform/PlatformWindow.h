#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>
#include <Runtime/Object/Object.h>
#include <Runtime/Window/WindowDesc.h>
#include <Runtime/Window/WindowEventDesc.h>

namespace Hollow
{
	class RUNTIME_API PlatformWindow : public Object
	{
	public:
		static sharedPtr<PlatformWindow> InitializeWindow(const WindowDesc& desc);

	public:
		PlatformWindow(const WindowDesc& desc);
		virtual ~PlatformWindow() override = default;

		const Vector2u& GetWindowSize() const { return mWindowSize; }
		const Vector2i& GetWindowPosition() const { return mWindowPosition; }
		const string& GetWindowTitle() const { return mWindowTitle; }
		const WindowMode& GetWindowMode() const { return mWindowMode; }

		virtual void SetWindowSize(Vector2u newSize) { mWindowSize = newSize; }
		virtual void SetWindowTitle(const string& newTitle) { mWindowTitle = newTitle; }
		virtual void SetWindowMode(WindowMode newMode) { mWindowMode = newMode; }
		virtual void SetWindowPosition(Vector2i newPosition) { mWindowPosition = newPosition; }

		virtual void Show() = 0;
		virtual void Hide() = 0;

		virtual void PollEvents();

	protected:
		void TriggerWindowEvent(const WindowEventDesc& desc);

	private:
		Vector2u mWindowSize;
		Vector2i mWindowPosition;
		string mWindowTitle;
		WindowMode mWindowMode;

		arrayList<WindowEventDesc> mEventQueue;
	};
}
