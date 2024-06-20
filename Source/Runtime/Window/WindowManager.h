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

		SharedPtr<PlatformWindow> InitializeWindow(const WindowDesc& desc)
		{
			if(mDefaultWindow == nullptr)
				return mDefaultWindow = PlatformWindow::InitializeWindow(desc);

			return mDefaultWindow;
		}

		SharedPtr<PlatformWindow> CreateExtraWindow(const WindowDesc& desc)
		{
			SharedPtr<PlatformWindow> window = PlatformWindow::InitializeWindow(desc);
			mExtraWindows.push_back(window);
			return window;
		}

		SharedPtr<PlatformWindow> GetDefaultWindow() { return mDefaultWindow; }
		DArray<SharedPtr<PlatformWindow>>& GetExtraWindows() { return mExtraWindows; }

	private:
		SharedPtr<PlatformWindow> mDefaultWindow;
		DArray<SharedPtr<PlatformWindow>> mExtraWindows;
	};
}
