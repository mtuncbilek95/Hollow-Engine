#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>

namespace Hollow
{
	class RUNTIME_API Semaphore : public GraphicsDeviceObject
	{
	public:
		Semaphore(GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice) {}
		virtual ~Semaphore() override = default;

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::Semaphore; }

		virtual void OnShutdown() noexcept override = 0;
	};
}