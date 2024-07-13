#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/IObject.h >
#include <Runtime/Graphics/Device/GraphicsDeviceObjectType.h>

namespace Hollow
{
	class RUNTIME_API GraphicsDevice;

	class RUNTIME_API GraphicsDeviceObject : public IObject
	{
	public:
		GraphicsDeviceObject(const SharedPtr<GraphicsDevice> device) : IObject(), mOwnerDevice(device) {}
		virtual ~GraphicsDeviceObject() override = default;

		FORCEINLINE SharedPtr<GraphicsDevice> GetOwnerDevice() const { return mOwnerDevice; }
		FORCEINLINE virtual GraphicsDeviceObjectType GetObjectType() const noexcept = 0;

	private:
		SharedPtr<GraphicsDevice> mOwnerDevice;
	};
}
