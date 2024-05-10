#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>

namespace Hollow
{
	class RUNTIME_API Semaphore : public GraphicsDeviceObject
	{
	public:
		Semaphore(const SharedPtr<GraphicsDevice> device) : GraphicsDeviceObject(device) {}
		virtual ~Semaphore() override = default;

		FORCEINLINE virtual GraphicsDeviceObjectType GetObjectType() const noexcept override final { return GraphicsDeviceObjectType::Semaphore; }

		virtual void OnShutdown() noexcept override = 0;

	private:
	};
}