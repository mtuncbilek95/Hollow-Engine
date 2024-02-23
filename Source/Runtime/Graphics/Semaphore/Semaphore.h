#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Semaphore/SemaphoreDesc.h>

namespace Hollow
{
	class RUNTIME_API Semaphore : public GraphicsDeviceObject
	{
	public:
		Semaphore(const SemaphoreDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), mSignalled(desc.bSignalled) {}
		virtual ~Semaphore() override = default;

		bool IsSignalled() const { return mSignalled; }

		FORCEINLINE virtual GraphicsDeviceObjectType GetDeviceObjectType() const noexcept { return GraphicsDeviceObjectType::Semaphore; }

		virtual void OnShutdown() noexcept override = 0;
	private:
		bool mSignalled;
	};
}