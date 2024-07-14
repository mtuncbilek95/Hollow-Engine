#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/IObject.h>

namespace Hollow
{
	class ENGINE_API GraphicsDevice;
	class ENGINE_API DeviceObject : public IObject
	{
	public:
		DeviceObject(SharedPtr<GraphicsDevice> pDevice) : mDevice(pDevice) {}
		virtual ~DeviceObject() override = default;

		SharedPtr<GraphicsDevice> GetOwnerDevice() const { return mDevice; }

	private:
		SharedPtr<GraphicsDevice> mDevice;
	};
}
