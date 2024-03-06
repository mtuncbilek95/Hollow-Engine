#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/Manager.h>

#include <Runtime/Window/Window.h>

namespace Hollow
{
	class RUNTIME_API WindowManager : public Manager<WindowManager>
	{
	public:
		WindowManager() = default;
		~WindowManager() = default;

		SharedPtr<Window> CreateAppWindow(const WindowDesc& desc)
		{
			if (mWindow == nullptr)
			{
				CORE_LOG(HE_WARNING, "WindowManager", "System window is not created yet. Creating new window...");
				mWindow = SharedPtr<Window>(new Window(desc));
			}
			return mWindow;
		}

		SharedPtr<Window>& GetMainWindow() { return mWindow; }

	private:
		SharedPtr<Window> mWindow;
	};
}
