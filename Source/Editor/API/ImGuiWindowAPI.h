#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/API.h>

#include <Editor/GuiWindow/GuiWindow.h>

namespace Hollow
{
	class EDITOR_API ImGuiWindowAPI : public API<ImGuiWindowAPI>
	{
		friend class GuiWindowModule;
	public:
		ImGuiWindowAPI() = default;
		~ImGuiWindowAPI() override = default;

		template<typename T, typename...Args>
		void RegisterGuiWindow(Args&&...args)
		{
			SharedPtr<T> window = MakeShared<T>(std::forward<Args>(args)...);
			mGuiWindows.push_back(window);
		}

		template<typename T>
		SharedPtr<T> GetGuiWindow(SharedPtr<GuiWindow> pWindow)
		{
			for (auto& window : mGuiWindows)
			{
				if (window->GetGuid() == pWindow->GetGuid())
					return window->GetSharedPtrAs<T>();
			}

			CORE_LOG(HE_ERROR, "%s GuiWindow [ID:%s] has not been found!", pWindow->mWindowName.c_str(), pWindow->GetGuid().ToString().c_str());
			return nullptr;
		}

		void UnregisterGuiWindow(const String& windowName)
		{
			for (auto it = mGuiWindows.begin(); it != mGuiWindows.end();)
			{
				if ((*it)->GetWindowName() == windowName)
					it = mGuiWindows.erase(it);
				else
					++it;
			}

			CORE_LOG(HE_ERROR, "%s GuiWindow has not been found!", windowName.c_str());
		}

		DArray<SharedPtr<GuiWindow>>& GetGuiWindows() { return mGuiWindows; }

		bool IsRequestQuit() { return mRequestQuit; }
		void RequestQuit() { mRequestQuit = true; }

	private:
		DArray<SharedPtr<GuiWindow>> mGuiWindows;
		bool mRequestQuit = false;
	};
}
