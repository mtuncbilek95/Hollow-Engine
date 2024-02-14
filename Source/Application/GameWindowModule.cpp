#include "GameWindowModule.h"
#include <Runtime/Application/Application.h>

namespace Hollow
{
	GameWindowModule::GameWindowModule()
	{
		SetModuleName("GameWindowModule");
	}

	void GameWindowModule::Start()
	{
	}

	void GameWindowModule::Update()
	{
		mGameWindow->PollMessages();

		if (mGameWindow->ShouldClose())
		{
			GetOwnerApplication()->RequestQuit();
			SendQuitReason("Requested Quit from Game Window Module.");
		}
	}

	void GameWindowModule::Stop()
	{
	}

	bool GameWindowModule::OnInitialize()
	{
		WindowDesc windowDesc;
		windowDesc.WindowPosition = { 1000, 100 };
		windowDesc.WindowSize = { 800, 600 };
		windowDesc.WindowTitle = "Hollow Engine";
		windowDesc.Windowed = true;

		mGameWindow = WindowManager::GetInstance().CreateAppWindow(windowDesc);
		return true;
	}

	void GameWindowModule::OnPreUpdate()
	{
	}

	void GameWindowModule::OnPostUpdate()
	{
	}

	void GameWindowModule::OnShutdown()
	{
		DEV_LOG(HE_INFO, "Game Window Module is shutting down.");
	}
}
