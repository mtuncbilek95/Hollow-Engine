#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/ManagerAPI.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>

namespace Hollow
{
	class RUNTIME_API GraphicsManager : public ManagerAPI<GraphicsManager>
	{
		friend class GraphicsDevice;
		friend class GraphicsQueue;

	public:
		GraphicsManager() : mGraphicsDevice(nullptr) {}
		~GraphicsManager() = default;

		GraphicsDevice* GetGraphicsDevice();

		sharedPtr<GraphicsQueue> GetPresentQueue() const noexcept;
		sharedPtr<GraphicsQueue> GetComputeQueue() const noexcept;
		sharedPtr<GraphicsQueue> GetTransferQueue() const noexcept;
		sharedPtr<GraphicsMemory> GetHostMemory() const noexcept;
		sharedPtr<GraphicsMemory> GetDeviceMemory() const noexcept;

		void CreateInternalResources();

	private:
		void SetGraphicsDevice(GraphicsDevice* graphicsDevice);

	private:
		GraphicsDevice* mGraphicsDevice;

		sharedPtr<GraphicsQueue> mPresentQueue;
		sharedPtr<GraphicsQueue> mComputeQueue;
		sharedPtr<GraphicsQueue> mTransferQueue;

		sharedPtr<GraphicsMemory> mHostMemory;
		sharedPtr<GraphicsMemory> mDeviceMemory;
	};
}
