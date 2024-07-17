#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Command/CmdPoolDesc.h>

namespace Hollow
{
	/**
	 * @class CmdPool
	 * @brief Abstract class for command pool
	 */
	class ENGINE_API CmdPool : public DeviceObject
	{
	public:
		CmdPool(const CmdPoolDesc& desc, const WeakPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice), mPoolType(desc.PoolType) {}
		virtual ~CmdPool() override = default;

		CmdPoolType GetPoolType() const noexcept { return mPoolType; }

	private:
		CmdPoolType mPoolType;
	};
}