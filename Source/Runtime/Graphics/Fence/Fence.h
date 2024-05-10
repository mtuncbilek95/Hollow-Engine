#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Fence/FenceDesc.h>

namespace Hollow
{
	class RUNTIME_API Fence : public GraphicsDeviceObject
	{
	public:
		Fence(const FenceDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), mSignaled(desc.bSignaled) 
		{}
		virtual ~Fence() override = default;

		bool IsSignaled() const noexcept { return mSignaled; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::Fence; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		bool mSignaled;
	};
}