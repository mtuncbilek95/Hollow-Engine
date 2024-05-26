#include "EditorPlayerWindowModule.h"

#include <Runtime/Platform/PlatformMonitor.h>
#include <Runtime/Window/WindowManager.h>
#include <Runtime/Application/Application.h>

namespace Hollow
{
	EditorPlayerWindowModule::EditorPlayerWindowModule()
	{
	}

	void EditorPlayerWindowModule::Start() noexcept
	{
		mWindow->Show();
	}

	void EditorPlayerWindowModule::Update() noexcept
	{
	}

	void EditorPlayerWindowModule::Stop() noexcept
	{
		mWindow->Hide();
		GetOwnerApp()->QuitReason("Window close has been requested.");
	}

	void EditorPlayerWindowModule::OnShutdown() noexcept
	{
	}

	bool EditorPlayerWindowModule::OnInitialize() noexcept
	{
		// Get the primary monitor
		auto primaryMonitor = PlatformMonitor::GetPrimaryMonitor();

		// Create a window
		WindowDesc desc = {};
		desc.WindowSize = { 1920, 1080 };
		desc.WindowPosition = { 0, 0 };
		desc.WindowTitle = "Hollow Engine";
		desc.WindowMode = WindowMode::Windowed;
		desc.pMonitor = primaryMonitor;

		mWindow = WindowManager::GetInstanceAPI().InitializeWindow(desc);

		if (!mWindow)
			return false;

		return true;
	}

	void EditorPlayerWindowModule::OnPreUpdate() noexcept
	{
		mWindow->PollEvents();
	}

	void EditorPlayerWindowModule::OnPostUpdate() noexcept
	{
	}
}
