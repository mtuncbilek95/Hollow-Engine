#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/API.h>

#include <Engine/Graphics/Instance/GraphicsInstanceDesc.h>

namespace Hollow
{
	class ENGINE_API GraphicsInstance;
	class ENGINE_API GraphicsDevice;
	class ENGINE_API Swapchain;

	class ENGINE_API GraphicsAPI : public API<GraphicsAPI>
	{
	public:
		~GraphicsAPI() override = default;

		SharedPtr<GraphicsInstance> CreateInstance(const GraphicsInstanceDesc& desc);
		SharedPtr<GraphicsInstance> GetInstance() const { return mInstance; }

		SharedPtr<GraphicsDevice> CreateDevice();
		SharedPtr<GraphicsDevice> GetDevice() const { return mDevice; }

		//SharedPtr<Swapchain> CreateSwapchain();
		SharedPtr<Swapchain> GetSwapchain() const { return mSwapchain; }

	private:
		SharedPtr<GraphicsInstance> mInstance;
		SharedPtr<GraphicsDevice> mDevice;
		SharedPtr<Swapchain> mSwapchain;
	};
}
