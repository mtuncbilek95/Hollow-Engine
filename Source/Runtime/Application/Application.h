#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/Object.h>
#include <Runtime/Application/ApplicationModule.h>

namespace Hollow
{
	class RUNTIME_API Application : public Object
	{
	public:
		Application() = default;
		virtual ~Application() override = default;

		template<typename ModuleType, typename... Args>
		void RegisterModule(Args&&... args)
		{
			auto pModule = std::make_shared<ModuleType>(std::forward<Args>(args)...);
			auto pApp = std::static_pointer_cast<Application>(GetSharedPtr());
			pModule->SetOwnerApplication(pApp);
			pModule->SetState(ApplicationModuleState::NeedValidation);
			mTotalModules.push_back(pModule);
			CORE_LOG(HE_INFO, "Application", "Module %s registered", pModule->GetModuleName().c_str());
		}

		void Run();

		void RequestQuit() { mQuitRequested = true; }
		void QuitReason(const String& reason);

		virtual void OnShutdown() noexcept override = 0;

	protected:
		virtual void OnInitialize() noexcept = 0;

	private:
		ArrayList<SharedPtr<ApplicationModule>> mTotalModules;
		HashMap<ApplicationModuleState, SharedPtr<ApplicationModule>> mHashModules;
		bool mQuitRequested = false;
	};
}
