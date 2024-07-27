#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Application/ApplicationModule.h>

namespace Hollow
{
	class ENGINE_API GraphicsDevice;
	class ENGINE_API Swapchain;
	class ENGINE_API Fence;
	class ENGINE_API GraphicsQueue;
	class ENGINE_API CmdBuffer;
	class ENGINE_API CmdPool;

	class EDITOR_API ImGuiBeginModule : public ApplicationModule
	{
	public:
		ImGuiBeginModule();
		virtual ~ImGuiBeginModule() override = default;

		virtual void Start() noexcept override;
		virtual void Update() noexcept override;
		virtual void Stop() noexcept override;

	protected:
		virtual bool OnInitialize() noexcept override;
		virtual void OnPreUpdate() noexcept override;
		virtual void OnPostUpdate() noexcept override;

	private:
		WeakPtr<GraphicsDevice> mDevice;
		WeakPtr<Swapchain> mSwapchain;
		WeakPtr<GraphicsQueue> mGraphicsQueue;

		SharedPtr<Fence> mFence;
		WeakPtr<CmdBuffer> mCmdBuffer;
	};
}