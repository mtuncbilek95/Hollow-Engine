#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Application/ApplicationModuleState.h>

namespace Hollow
{
	class Application;
	class RUNTIME_API ApplicationModule
	{
		friend class Application;
	public:
		ApplicationModule() = default;
		virtual ~ApplicationModule() = default;

		FORCEINLINE ApplicationModuleState GetState() const noexcept { return mState; }
		FORCEINLINE String GetModuleName() const noexcept { return mModuleName; }
		FORCEINLINE Application* GetOwnerApplication() const noexcept { return mOwnerApplication; }

		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void Stop() = 0;

	protected:
		virtual bool OnInitialize() = 0;
		virtual void OnPreUpdate() = 0;
		virtual void OnPostUpdate() = 0;
		virtual void OnShutdown() = 0;

		void SetModuleName(const String& name) { mModuleName = name; }
		void SendQuitReason(const String& reason);

	private:
		void SetOwnerApplication(Application* app) { mOwnerApplication = app; }
		void SetState(ApplicationModuleState state) { mState = state; }

		Application* mOwnerApplication = nullptr;
		ApplicationModuleState mState = ApplicationModuleState::Unknown;
		String mModuleName = "Unknown";
	};
}
