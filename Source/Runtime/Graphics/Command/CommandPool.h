#pragma once
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Command/CommandPoolDesc.h>

namespace Hollow
{
	/**
	 * @class CommandPool
	 * @brief Abstract class for command pool
	 */
	class RUNTIME_API CommandPool : public GraphicsDeviceObject
	{
	public:
		CommandPool(const CommandPoolDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), mPoolType(desc.PoolType) {}
		virtual ~CommandPool() override = default;

		CommandPoolType GetPoolType() const noexcept { return mPoolType; }

		// Inherited via GraphicsDeviceObject
		FORCEINLINE virtual GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::CommandPool; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		CommandPoolType mPoolType;
	};
}