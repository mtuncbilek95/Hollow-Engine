#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Application/ApplicationModule.h>
#include <Runtime/Window/WindowManager.h>

namespace Hollow
{
	class GAME_API GameWindowModule : public ApplicationModule
	{
	public:
		GameWindowModule();
		virtual ~GameWindowModule() override = default;

		void Start() override;
		void Update() override;
		void Stop() override;

	protected:
		bool OnInitialize() override;
		void OnPreUpdate() override;
		void OnPostUpdate() override;
		void OnShutdown() override;

	private:
		SharedPtr<Window> mGameWindow;
	};
}
