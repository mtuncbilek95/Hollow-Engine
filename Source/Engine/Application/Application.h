#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/IObject.h >
#include <Engine/Application/ApplicationModule.h>

namespace Hollow
{
	class ENGINE_API Application : public IObject
	{
	public:
		Application() = default;
		virtual ~Application() override = default;

		template<typename ModuleType, typename... Args>
		void RegisterModule(Args&&... args)
		{
			SharedPtr<ModuleType> pModule = MakeShared<ModuleType>(std::forward<Args>(args)...);
			pModule->SetOwnerApplication(GetSharedPtrAs<Application>());
			pModule->SetState(ApplicationModuleState::NeedValidation);
			mTotalModules.push_back(pModule);
			CORE_LOG(HE_INFO, "Application", "Module %s registered", pModule->GetModuleName().c_str());
		}

		void Run();

		void RequestQuit() { mQuitRequested = true; }
		void QuitReason(const String& reason);

	protected:
		virtual void OnInitialize() noexcept = 0;

	private:
		DArray<SharedPtr<ApplicationModule>> mTotalModules;
		bool mQuitRequested = false;
	};
}
