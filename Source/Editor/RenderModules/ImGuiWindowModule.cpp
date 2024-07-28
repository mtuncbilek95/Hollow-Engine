#include "ImGuiWindowModule.h"

#include <Engine/Application/Application.h>
#include <Editor/API/ImGuiWindowAPI.h>

#include <Editor/GuiWindow/Built-in/DockWindow.h>
#include <Editor/GuiWindow/Built-in/ContentWindow.h>
#include <Editor/GuiWindow/Built-in/PropertyWindow.h>
#include <Editor/GuiWindow/Built-in/SceneWindow.h>
#include <Editor/GuiWindow/Built-in/WorldWindow.h>

namespace Hollow
{
	ImGuiWindowModule::ImGuiWindowModule()
	{
		SetModuleName("ImGuiWindowModule");
		mImGuiAPI = ImGuiWindowAPI::GetAPI();

		mImGuiAPI->RegisterGuiWindow<DockWindow>();
		mImGuiAPI->RegisterGuiWindow<ContentWindow>();
		mImGuiAPI->RegisterGuiWindow<PropertyWindow>();
		mImGuiAPI->RegisterGuiWindow<SceneWindow>();
		mImGuiAPI->RegisterGuiWindow<WorldWindow>();

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

		if (mImGuiAPI->IsRequestQuit())
		{
			GetOwnerApp().lock()->RequestQuit();
			GetOwnerApp().lock()->QuitReason("Quit from MenuBar.");
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
