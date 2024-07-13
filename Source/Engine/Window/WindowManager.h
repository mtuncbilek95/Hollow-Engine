#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/API.h>
#include <Engine/Platform/PlatformWindow.h>

namespace Hollow
{
	class ENGINE_API WindowManager : public API<WindowManager>
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