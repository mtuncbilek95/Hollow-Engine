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

		DArray<SharedPtr<GuiWindow>>& GetGuiWindows() { return mGuiWindows; }

	private:
		DArray<SharedPtr<GuiWindow>> mGuiWindows;
	};
}
