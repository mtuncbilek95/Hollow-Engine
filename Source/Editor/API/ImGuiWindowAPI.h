#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/API.h>

#include <Editor/GuiWindow/GuiWindow.h>

namespace Hollow
{
	class ImGuiWindowAPI : public API<ImGuiWindowAPI>
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
		SharedPtr<T> GetGuiWindow(const String& windowName)
		{
			for (auto& window : mGuiWindows)
			{
				if (window->GetWindowName() == windowName)
					return std::static_pointer_cast<T>(window);
			}
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
		}

		DArray<SharedPtr<GuiWindow>>& GetGuiWindows() { return mGuiWindows; }

		bool IsRequestQuit() { return mRequestQuit; }
		void RequestQuit() { mRequestQuit = true; }

	private:
		DArray<SharedPtr<GuiWindow>> mGuiWindows;
		bool mRequestQuit = false;
	};
}
