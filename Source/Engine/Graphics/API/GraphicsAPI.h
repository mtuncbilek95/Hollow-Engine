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

		WeakPtr<GraphicsInstance> CreateInstance(const GraphicsInstanceDesc& desc);
		WeakPtr<GraphicsInstance> GetInstance() const { return mInstance; }

		WeakPtr<GraphicsDevice> CreateDevice();
		WeakPtr<GraphicsDevice> GetDevice() const { return mDevice; }

		WeakPtr<GraphicsQueue> GetGraphicsQueue() const { return mGraphicsQueue; }
		WeakPtr<GraphicsQueue> GetComputeQueue() const { return mComputeQueue; }
		WeakPtr<GraphicsQueue> GetTransferQueue() const { return mTransferQueue; }

		WeakPtr<GraphicsMemory> GetAvailableDeviceMemory(u64 requestedSize);
		WeakPtr<GraphicsMemory> GetAvailableHostMemory(u64 requestedSize);

		WeakPtr<Swapchain> CreateSwapchain(const SwapchainDesc& desc);
		WeakPtr<Swapchain> GetSwapchain() const { return mSwapchain; }

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
