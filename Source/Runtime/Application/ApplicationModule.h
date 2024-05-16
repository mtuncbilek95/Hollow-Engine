#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/Object.h>
#include <Runtime/Application/ApplicationModuleState.h>

namespace Hollow
{
	class RUNTIME_API ApplicationModule : public Object
	{
		friend class Application;

	public:
		ApplicationModule() = default;
		virtual ~ApplicationModule() override = default;

		FORCEINLINE SharedPtr<Application> GetOwnerApp() const { return mOwnerApp; }
		FORCEINLINE String GetModuleName() const { return mModuleName; }
		FORCEINLINE ApplicationModuleState GetState() const { return mState; }

		virtual void Start() noexcept = 0;
		virtual void Update() noexcept = 0;
		virtual void Stop() noexcept = 0;

		void OnShutdown() noexcept override = 0;

	protected:
		virtual bool OnInitialize() noexcept = 0;
		virtual void OnPreUpdate() noexcept = 0;
		virtual void OnPostUpdate() noexcept = 0;

		void SetOwnerApplication(SharedPtr<Application> app) { mOwnerApp = app; }
		void SetState(ApplicationModuleState state) { mState = state; }
		void SetModuleName(const String& name) { mModuleName = name; }

	private:
		SharedPtr<Application> mOwnerApp;
		ApplicationModuleState mState = ApplicationModuleState::NeedValidation;
		String mModuleName;
	};
}
