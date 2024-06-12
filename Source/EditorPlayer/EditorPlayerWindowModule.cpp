#include "EditorPlayerWindowModule.h"

#include <Runtime/Window/WindowManager.h>
#include <Runtime/Application/Application.h>

namespace Hollow
{
	EditorPlayerWindowModule::EditorPlayerWindowModule()
	{
	}

	void EditorPlayerWindowModule::Start() noexcept
	{
	}

	void EditorPlayerWindowModule::Update() noexcept
	{
	}

	void EditorPlayerWindowModule::Stop() noexcept
	{
		GetOwnerApp()->QuitReason("Window close has been requested.");
	}

	void EditorPlayerWindowModule::OnShutdown() noexcept
	{
	}

	bool EditorPlayerWindowModule::OnInitialize() noexcept
	{
		// Create a window
		WindowDesc desc = {};
		desc.WindowSize = { 1920, 1080 };
		desc.WindowPosition = { 0, 0 };
		desc.WindowTitle = "Hollow Engine";
		desc.WindowMode = WindowMode::Windowed;
		
		mWindow = WindowManager::GetInstanceAPI().InitializeWindow(desc);

		if (!mWindow)
			return false;

		return true;
	}

	void EditorPlayerWindowModule::OnPreUpdate() noexcept
	{
	}

	void EditorPlayerWindowModule::OnPostUpdate() noexcept
	{
	}
}
