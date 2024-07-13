#include "Application.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

namespace Hollow
{
	void Application::Run()
	{
		OnInitialize();

		for (auto& pModule : mTotalModules)
		{
			if (pModule->GetState() == ApplicationModuleState::NeedValidation)
			{
				if (pModule->OnInitialize())
				{
					CORE_LOG(HE_INFO, "Application", "Module %s validated", pModule->GetModuleName().c_str());
					pModule->SetState(ApplicationModuleState::Validated);
					mHashModules.insert({ pModule->GetState(), pModule });
				}
				else
				{
					CORE_LOG(HE_INFO, "Application", "Module %s invalidated", pModule->GetModuleName().c_str());
					pModule->SetState(ApplicationModuleState::Invalidated);
					mHashModules.insert({ pModule->GetState(), pModule });
				}
			}
		}

		for (auto& [key, value] : mHashModules)
		{
			if (key == ApplicationModuleState::Validated)
			{
				value->Start();
			}
		}

		while (!mQuitRequested)
		{
			for (auto& [key, value] : mHashModules)
			{
				if (key == ApplicationModuleState::Validated)
				{
					value->OnPreUpdate();
				}
			}

			for (auto& [key, value] : mHashModules)
			{
				if (key == ApplicationModuleState::Validated)
				{
					value->Update();
				}
			}

			for (auto& [key, value] : mHashModules)
			{
				if (key == ApplicationModuleState::Validated)
				{
					value->OnPostUpdate();
				}
			}

			if (GetAsyncKeyState(VK_ESCAPE))
			{
				QuitReason("User Pressed Escape Key");
				mQuitRequested = true;
			}
		}

		for (auto& [key, value] : mHashModules)
		{
			if (key == ApplicationModuleState::Validated)
				value->Stop();
		}
	}

	void Application::QuitReason(const String& reason)
	{
		CORE_LOG(HE_WARNING, "Application Quit Reason", "%s", reason.c_str());
	}
}
