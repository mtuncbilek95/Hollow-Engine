#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Application/ApplicationModule.h>

namespace Hollow
{
	class EDITORPLAYER_API EditorPlayerGraphicsModule : public ApplicationModule
	{
	public:
		EditorPlayerGraphicsModule();
		~EditorPlayerGraphicsModule() override = default;

		virtual void Start() noexcept override;
		virtual void Update() noexcept override;
		virtual void Stop() noexcept override;

		void OnShutdown() noexcept override;

	protected:
		virtual bool OnInitialize() noexcept override;
		virtual void OnPreUpdate() noexcept override;
		virtual void OnPostUpdate() noexcept override;

	private:
		SharedPtr<class GraphicsInstance> mGraphicsInstance;
		SharedPtr<class GraphicsDevice> mGraphicsDevice;
		SharedPtr<class Swapchain> mSwapchain;
	};
}
