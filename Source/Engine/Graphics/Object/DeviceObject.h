#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/IObject.h>

namespace Hollow
{
	class GraphicsDevice;
	class DeviceObject : public IObject
	{
	public:
		DeviceObject(WeakPtr<GraphicsDevice> pDevice) : mDevice(pDevice) {}
		virtual ~DeviceObject() override = default;

		WeakPtr<GraphicsDevice> GetOwnerDevice() const { return mDevice; }

	private:
		WeakPtr<GraphicsDevice> mDevice;
	};
}
