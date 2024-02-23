#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Fence/FenceDesc.h>

namespace Hollow
{
	class RUNTIME_API Fence : public GraphicsDeviceObject
	{
	public:
		Fence(const FenceDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), mSignalled(desc.bSignalled) {}
		virtual ~Fence() override = default;

		FORCEINLINE virtual GraphicsDeviceObjectType GetDeviceObjectType() const noexcept override final { return GraphicsDeviceObjectType::Fence; }

		virtual void OnShutdown() noexcept override = 0;
	private:
		bool mSignalled;
	};
}