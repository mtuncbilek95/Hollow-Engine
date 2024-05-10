#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/ManagerAPI.h>
#include <Runtime/Platform/PlatformWindow.h>

namespace Hollow
{
	class RUNTIME_API WindowManager : public ManagerAPI<WindowManager>
	{
	public:
		WindowManager() = default;
		~WindowManager() = default;

		sharedPtr<PlatformWindow> InitializeWindow(const WindowDesc& desc)
		{
			if(mDefaultWindow == nullptr)
				return mDefaultWindow = PlatformWindow::InitializeWindow(desc);

			return mDefaultWindow;
		}

		sharedPtr<PlatformWindow> CreateExtraWindow(const WindowDesc& desc)
		{
			sharedPtr<PlatformWindow> window = PlatformWindow::InitializeWindow(desc);
			mExtraWindows.push_back(window);
			return window;
		}

		sharedPtr<PlatformWindow> GetDefaultWindow() { return mDefaultWindow; }
		arrayList<sharedPtr<PlatformWindow>>& GetExtraWindows() { return mExtraWindows; }

	private:
		sharedPtr<PlatformWindow> mDefaultWindow;
		arrayList<sharedPtr<PlatformWindow>> mExtraWindows;
	};
}
