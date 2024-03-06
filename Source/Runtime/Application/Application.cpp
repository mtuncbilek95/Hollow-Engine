#include "Application.h"

#include "Windows.h"

namespace Hollow
{
	void Application::Run()
	{
		// Initialize sub application
		OnInitialize();

		// Initialize modules
		for (auto& module : mModules)
		{
			// Try to initialize module
			if (module->GetState() == ApplicationModuleState::NeedValidation)
			{
				if (module->OnInitialize())
				{
					module->SetState(ApplicationModuleState::Validated);
					CORE_LOG(HE_VERBOSE, "Validation", "%s has been validated.", module->GetModuleName().c_str());
				}
				else
				{
					module->SetState(ApplicationModuleState::Invalidated);
					CORE_LOG(HE_WARNING, "Validation", "%s has been invalidated. You won't be able to use this module.", module->GetModuleName().c_str());
				}
			}

			// If module is validated, start it
			if (module->GetState() == ApplicationModuleState::Validated)
			{
				module->Start();
			}
		}

		// Loop validated modules until quit is requested
		while (!mQuitRequested)
		{
			for (auto& module : mModules)
			{
				if (module->GetState() == ApplicationModuleState::Validated)
				{
					module->OnPreUpdate();
				}
			}

			for (auto& module : mModules)
			{
				if (module->GetState() == ApplicationModuleState::Validated)
				{
					module->Update();
				}
			}

			for (auto& module : mModules)
			{
				if (module->GetState() == ApplicationModuleState::Validated)
				{
					module->OnPostUpdate();
				}
			}

			if (GetAsyncKeyState(VK_ESCAPE))
			{
				QuitReason("Requested Quit from Application Escape Button.");
				mQuitRequested = true;
			}
		}

		for (int i = static_cast<int>(mModules.size()) - 1; i >= 0; i--)
		{
			mModules[i]->Stop();
			mModules[i]->OnShutdown();
		}

		CORE_LOG(HE_WARNING, "Application", "Shutting down.");
	}

	void Application::QuitReason(const String& reason)
	{
		CORE_LOG(HE_WARNING, "Quit Reason", "%s", reason.c_str());
	}
}
