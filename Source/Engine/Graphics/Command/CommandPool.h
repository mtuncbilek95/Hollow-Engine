#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Command/CommandPoolDesc.h>

namespace Hollow
{
	/**
	 * @class CommandPool
	 * @brief Abstract class for command pool
	 */
	class ENGINE_API CommandPool : public DeviceObject
	{
	public:
		CommandPool(const CommandPoolDesc& desc, const SharedPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice), mPoolType(desc.PoolType) {}
		virtual ~CommandPool() override = default;

		CommandPoolType GetPoolType() const noexcept { return mPoolType; }

	private:
		CommandPoolType mPoolType;
	};
}