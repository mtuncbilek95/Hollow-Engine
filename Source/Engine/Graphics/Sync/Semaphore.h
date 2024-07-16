#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>

namespace Hollow
{
	class ENGINE_API GraphicsDevice;
	class ENGINE_API Semaphore : public DeviceObject
	{
	public:
		Semaphore(SharedPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice){}
		~Semaphore() override = default;
	};
}
