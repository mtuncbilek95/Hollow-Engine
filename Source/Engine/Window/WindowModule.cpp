#include "WindowModule.h"

#include <Engine/Window/WindowAPI.h>
#include <Engine/Application/Application.h>

#include <nlohmann/json.hpp>
#include <Engine/Platform/PlatformFile.h>
#include <Engine/Platform/API/PlatformAPI.h>

namespace Hollow
{
	// TODO: Its not permanent, will be changed
	WindowDesc GetJson()
	{
		nlohmann::json pJson;
		String path = PlatformAPI::GetAPI()->GetEngineSourcePath() + "Config/EditorConfig.json";
		String jString;
		PlatformFile::Read(path, jString);
		pJson = nlohmann::json::parse(jString);

		WindowDesc windowDesc = {};
		windowDesc.WindowMode = static_cast<WindowMode>(pJson.at("EditorConfig").at("EditorWindow").at("WindowMode").get<int>());
		windowDesc.WindowSize = Vec2i(100, 100);
		windowDesc.WindowSize = Vec2u(pJson.at("EditorConfig").at("EditorWindow").at("Resolution")[0].get<int>(), pJson.at("EditorConfig").at("EditorWindow").at("Resolution")[1].get<int>());
		windowDesc.WindowTitle = pJson.at("EditorConfig").at("EditorTitle").get<String>() + pJson.at("EditorConfig").at("EditorVersion").get<String>();

		CORE_LOG(HE_INFO, "Currently using: %s", windowDesc.WindowTitle.c_str());

		return windowDesc;
	}

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
		//WindowDesc windowDesc = {};
		//windowDesc.WindowMode = WindowMode::Windowed; // TODO: Get from config
		//windowDesc.WindowPosition = Vec2i(100, 100); // TODO: Get from config
		//windowDesc.WindowSize = Vec2u(2560, 1440); // TODO: Get from config
		//windowDesc.WindowTitle = "Hollow Editor"; // TODO: Get from config

		auto tempRefWindow = mWindowAPI->InitializeWindow(GetJson());
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
