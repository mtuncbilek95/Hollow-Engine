#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Application/ApplicationModule.h>

namespace Hollow
{
	class RUNTIME_API Application
	{
	public:
		Application() = default;
		virtual ~Application() = default; 

		template<typename ModuleType, typename... Args>
		void RegisterModule(Args... args)
		{
			auto module = std::make_shared<ModuleType>(args...);
			module->SetOwnerApplication(this);
			module->SetState(ApplicationModuleState::NeedValidation);

			mModules.push_back(module);

			CORE_LOG(HE_INFO, "Application", "%s has been registered", module->GetModuleName().c_str());
		}

		void Run();

		void RequestQuit() { mQuitRequested = true; }
		void QuitReason(const String& reason);

	protected:
		virtual void OnInitialize() = 0;

	private:
		ArrayList<SharedPtr<ApplicationModule>> mModules;
		bool mQuitRequested = false;
	};
}
