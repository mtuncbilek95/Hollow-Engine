#include "WindowModule.h"

#include <Engine/Window/WindowAPI.h>
#include <Engine/Application/Application.h>

namespace Hollow
{
	WindowModule::WindowModule()
	{
		SetModuleName("WindowModule");
		mWindowAPI = WindowAPI::GetAPI();
	}

	WindowModule::~WindowModule()
	{
	}

	void WindowModule::Start() noexcept
	{
		mWindowAPI->GetDefaultWindow()->Show();
	}

	void WindowModule::Update() noexcept
	{
	}

	void WindowModule::Stop() noexcept
	{
		mWindowAPI->GetDefaultWindow()->Hide();
	}

	bool WindowModule::OnInitialize() noexcept
	{
		WindowDesc windowDesc = {};
		windowDesc.WindowMode = WindowMode::Windowed; // TODO: Get from config
		windowDesc.WindowPosition = Vec2i(100, 100); // TODO: Get from config
		windowDesc.WindowSize = Vec2u(2560, 1440); // TODO: Get from config
		windowDesc.WindowTitle = "Hollow Editor"; // TODO: Get from config

		auto tempRefWindow = mWindowAPI->InitializeWindow(windowDesc);
		if (tempRefWindow.expired())
			return false;

		return true;
	}

	void WindowModule::OnPreUpdate() noexcept
	{
		mWindowAPI->GetDefaultWindow()->PollEvents();

		if (mWindowAPI->GetDefaultWindow()->IsClosed())
		{
			GetOwnerApp().lock()->RequestQuit();
			GetOwnerApp().lock()->QuitReason("Window has been forced to be closed!");
		}
	}

	void WindowModule::OnPostUpdate() noexcept
	{
	}
}
