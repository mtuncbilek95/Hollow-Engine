#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/API.h>

#include <Engine/Graphics/Instance/GraphicsInstanceDesc.h>
#include <Engine/Graphics/Swapchain/SwapchainDesc.h>

namespace Hollow
{
	class ENGINE_API GraphicsInstance;
	class ENGINE_API GraphicsDevice;
	class ENGINE_API GraphicsQueue;
	class ENGINE_API GraphicsMemory;
	class ENGINE_API Swapchain;

	class ENGINE_API GraphicsAPI : public API<GraphicsAPI>
	{
	public:
		~GraphicsAPI() override = default;

		SharedPtr<GraphicsInstance> CreateInstance(const GraphicsInstanceDesc& desc);
		SharedPtr<GraphicsInstance> GetInstance() const { return mInstance; }

		SharedPtr<GraphicsDevice> CreateDevice();
		SharedPtr<GraphicsDevice> GetDevice() const { return mDevice; }

		SharedPtr<GraphicsQueue> GetGraphicsQueue() const { return mGraphicsQueue; }
		SharedPtr<GraphicsQueue> GetComputeQueue() const { return mComputeQueue; }
		SharedPtr<GraphicsQueue> GetTransferQueue() const { return mTransferQueue; }

		SharedPtr<GraphicsMemory> GetAvailableDeviceMemory(u64 requestedSize);
		SharedPtr<GraphicsMemory> GetAvailableHostMemory(u64 requestedSize);

		SharedPtr<Swapchain> CreateSwapchain(const SwapchainDesc& desc);
		SharedPtr<Swapchain> GetSwapchain() const { return mSwapchain; }

	private:
		SharedPtr<GraphicsInstance> mInstance;
		SharedPtr<GraphicsDevice> mDevice;
		SharedPtr<Swapchain> mSwapchain;

		SharedPtr<GraphicsQueue> mGraphicsQueue;
		SharedPtr<GraphicsQueue> mComputeQueue;
		SharedPtr<GraphicsQueue> mTransferQueue;

		List<SharedPtr<GraphicsMemory>> mDeviceMemChunks;
		List<SharedPtr<GraphicsMemory>> mHostMemChunks;
	};
}
