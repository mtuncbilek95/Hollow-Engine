#include "ImGuiWindowModule.h"

#include <Editor/API/ImGuiWindowAPI.h>

#include <Editor/GuiWindow/Built-in/DockWindow.h>

namespace Hollow
{
	ImGuiWindowModule::ImGuiWindowModule()
	{
		SetModuleName("ImGuiWindowModule");
		mImGuiAPI = ImGuiWindowAPI::GetAPI();

		mImGuiAPI->RegisterGuiWindow<DockWindow>();
	}

	void ImGuiWindowModule::Start() noexcept
	{
		auto windows = mImGuiAPI->GetGuiWindows();

		for (auto& window : windows)
		{
			window->OnInitialize();
		}
	}

	void ImGuiWindowModule::Update() noexcept
	{
		auto windows = mImGuiAPI->GetGuiWindows();

		for (auto& window : windows)
		{
			window->OnUpdate();
		}

		for (auto& window : windows)
		{
			window->OnPaint();
		}
	}

	void ImGuiWindowModule::Stop() noexcept
	{
	}

	bool ImGuiWindowModule::OnInitialize() noexcept
	{
		return true;
	}

	void ImGuiWindowModule::OnPreUpdate() noexcept
	{
	}

	void ImGuiWindowModule::OnPostUpdate() noexcept
	{
	}
}
