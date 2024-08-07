#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/API.h>

#include <Engine/Graphics/Instance/GraphicsInstanceDesc.h>
#include <Engine/Graphics/Swapchain/SwapchainDesc.h>

namespace Hollow
{
	class GraphicsInstance;
	class GraphicsDevice;
	class GraphicsQueue;
	class GraphicsMemory;
	class Swapchain;

	class GraphicsAPI : public API<GraphicsAPI>
	{
		friend class GraphicsModule;
	public:
		~GraphicsAPI() override = default;

		WeakPtr<GraphicsInstance> GetInstance() const { return mInstance; }
		WeakPtr<GraphicsDevice> GetDevice() const { return mDevice; }

		WeakPtr<GraphicsQueue> GetGraphicsQueue() const { return mGraphicsQueue; }
		WeakPtr<GraphicsQueue> GetComputeQueue() const { return mComputeQueue; }
		WeakPtr<GraphicsQueue> GetTransferQueue() const { return mTransferQueue; }

		WeakPtr<GraphicsMemory> GetAvailableDeviceMemory(u64 requestedSize);
		WeakPtr<GraphicsMemory> GetAvailableHostMemory(u64 requestedSize);

		WeakPtr<Swapchain> GetSwapchain() const { return mSwapchain; }

	private:
		WeakPtr<GraphicsInstance> CreateInstance(const GraphicsInstanceDesc& desc);
		WeakPtr<GraphicsDevice> CreateDevice();
		WeakPtr<Swapchain> CreateSwapchain(const SwapchainDesc& desc);

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
