#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/IObject.h>

namespace Hollow
{
	class ENGINE_API GraphicsDevice;
	class ENGINE_API DeviceObject : public IObject
	{
	public:
		DeviceObject(WeakPtr<GraphicsDevice> pDevice) : mDevice(pDevice) {}
		virtual ~DeviceObject() override = default;

		WeakPtr<GraphicsDevice> GetOwnerDevice() const { return mDevice; }

	private:
		WeakPtr<GraphicsDevice> mDevice;
	};
}
