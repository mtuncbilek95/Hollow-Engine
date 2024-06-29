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

		SharedPtr<GraphicsDevice> GetDefDevice() const noexcept { return mGraphicsDevice; }

		SharedPtr<GraphicsQueue> GetPresentQueue() const noexcept { return mPresentQueue; }
		SharedPtr<GraphicsQueue> GetComputeQueue() const noexcept { return mComputeQueue; }
		SharedPtr<GraphicsQueue> GetTransferQueue() const noexcept { return mTransferQueue; }

		SharedPtr<GraphicsMemory> GetHostMemory() const noexcept { return mHostMemory; }
		SharedPtr<GraphicsMemory> GetDeviceMemory() const noexcept { return mDeviceMemory; }

		int GetMsaaSamples() const noexcept { return mMsaaSamples; }

	private:
		void SetGraphicsDevice(SharedPtr<GraphicsDevice> device);

	private:
		SharedPtr<GraphicsDevice> mGraphicsDevice;

		SharedPtr<GraphicsQueue> mPresentQueue;
		SharedPtr<GraphicsQueue> mComputeQueue;
		SharedPtr<GraphicsQueue> mTransferQueue;

		SharedPtr<GraphicsMemory> mHostMemory;
		SharedPtr<GraphicsMemory> mDeviceMemory;

		int mMsaaSamples = 1;
	};
}
