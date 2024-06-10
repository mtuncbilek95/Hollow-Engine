#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/ManagerAPI.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>

namespace Hollow
{
	class RUNTIME_API GraphicsManager final : public ManagerAPI<GraphicsManager>
	{
		friend class GraphicsInstance;
	public:
		GraphicsManager() = default;
		~GraphicsManager() override = default;

		// The budget memories are in megabytes
		void SetMemoryBudget(const u64 hostMemoryBudget, const u64 deviceMemoryBudget) noexcept;
		void SetResourceBudget(const u64 resourceCount, const u64 setCount) noexcept;

		SharedPtr<GraphicsDevice> GetDefaultDevice() const noexcept { return mGraphicsDevice; }

		SharedPtr<GraphicsQueue> GetDefaultPresentQueue() const noexcept { return mPresentQueue; }
		SharedPtr<GraphicsQueue> GetDefaultComputeQueue() const noexcept { return mComputeQueue; }
		SharedPtr<GraphicsQueue> GetDefaultTransferQueue() const noexcept { return mTransferQueue; }

		SharedPtr<GraphicsMemory> GetDefaultHostMemory() const noexcept { return mHostMemory; }
		SharedPtr<GraphicsMemory> GetDefaultDeviceMemory() const noexcept { return mDeviceMemory; }

	private:
		void SetGraphicsDevice(SharedPtr<GraphicsDevice> device);

	private:
		SharedPtr<GraphicsDevice> mGraphicsDevice;

		SharedPtr<GraphicsQueue> mPresentQueue;
		SharedPtr<GraphicsQueue> mComputeQueue;
		SharedPtr<GraphicsQueue> mTransferQueue;

		SharedPtr<GraphicsMemory> mHostMemory;
		SharedPtr<GraphicsMemory> mDeviceMemory;

		SharedPtr<DescriptorPool> mDescriptorPool;
	};
}
