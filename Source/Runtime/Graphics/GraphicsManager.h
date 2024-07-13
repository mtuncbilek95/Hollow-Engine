#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/API.h>

namespace Hollow
{
		class RUNTIME_API GraphicsManager : public API<GraphicsManager>
	{
	public:
		GraphicsManager() = default;
		~GraphicsManager() = default;



	private:
		SharedPtr<GraphicsInstance> mGraphicsInstance;
		SharedPtr<GraphicsDevice> mGraphicsDevice;
		SharedPtr<Swapchain> mSwapchain;

		SharedPtr<GraphicsQueue> mGraphicsQueue;
		SharedPtr<ComputeQueue> mComputeQueue;
		SharedPtr<TransferQueue> mTransferQueue;

		List<SharedPtr<GraphicsMemory>> mDeviceMemory;
		List<SharedPtr<GraphicsMemory>> mHostMemory;
	};
