#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Application/ApplicationModule.h>

namespace Hollow
{
	class GraphicsDevice;
	class Swapchain;
	class Fence;
	class GraphicsQueue;
	class CmdBuffer;
	class CmdPool;

	class ImGuiEndModule : public ApplicationModule
	{
	public:
		ImGuiEndModule();
		virtual ~ImGuiEndModule() override = default;

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