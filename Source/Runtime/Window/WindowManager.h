#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/API.h>
#include <Runtime/Platform/PlatformWindow.h>

namespace Hollow
{
	class RUNTIME_API WindowManager : public API<WindowManager>
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

		SharedPtr<PlatformWindow> GetDefaultWindow() { return mDefaultWindow; }

	private:
		SharedPtr<PlatformWindow> mDefaultWindow;
	};
}
