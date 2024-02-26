#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Command/CommandPoolDesc.h>

namespace Hollow
{
	class RUNTIME_API CommandPool : public GraphicsDeviceObject
	{
	public:
		CommandPool(const CommandPoolDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), mType(desc.Type) {}
		virtual ~CommandPool() override = default;

		CommandPoolType GetType() const { return mType; }

		FORCEINLINE virtual GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::CommandPool; }

		virtual void OnShutdown() noexcept override = 0;
	private:
		CommandPoolType mType;
	};
}
