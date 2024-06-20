#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Fence/FenceDesc.h>

namespace Hollow
{
	/**
	 * @class Fence
	 * @brief Represents the sync object that can be used to synchronize CPU and GPU.
	 */
	class RUNTIME_API Fence : public GraphicsDeviceObject
	{
	public:
		Fence(const FenceDesc& desc, SharedPtr<GraphicsDevice> device) : GraphicsDeviceObject(device), mSignalled(desc.bSignalled) {}
		virtual ~Fence() override = default;

		bool IsSignalled() const noexcept { return mSignalled; }

		FORCEINLINE GraphicsDeviceObjectType GetObjectType() const noexcept override final { return GraphicsDeviceObjectType::Fence; }

	private:
		bool mSignalled;
	};
}
