#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/Object.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObjectType.h>

namespace Hollow
{
	class RUNTIME_API GraphicsDevice;

	class RUNTIME_API GraphicsDeviceObject : public Object
	{
	public:
		GraphicsDeviceObject(const SharedPtr<GraphicsDevice> device) : Object(), mOwnerDevice(device) {}
		virtual ~GraphicsDeviceObject() override = default;

		FORCEINLINE SharedPtr<GraphicsDevice> GetOwnerDevice() const { return mOwnerDevice; }
		FORCEINLINE virtual GraphicsDeviceObjectType GetObjectType() const noexcept = 0;

		virtual void OnShutdown() override = 0;
	

	private:
		SharedPtr<GraphicsDevice> mOwnerDevice;
	};
}
