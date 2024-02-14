#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Application/ApplicationModule.h>

#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>

namespace Hollow
{
	class GAME_API GameGraphicsModule : public ApplicationModule
	{
	public:
		GameGraphicsModule();
		virtual ~GameGraphicsModule() override = default;

		void Start() override;
		void Update() override;
		void Stop() override;

	protected:
		bool OnInitialize() override;
		void OnPreUpdate() override;
		void OnPostUpdate() override;
		void OnShutdown() override;

	private:
		SharedPtr<GraphicsDevice> mGraphicsDevice;
		SharedPtr<GraphicsAdapter> mAdapter;
		SharedPtr<GraphicsInstance> mInstance;
		SharedPtr<Swapchain> mSwapchain;
	};
}
